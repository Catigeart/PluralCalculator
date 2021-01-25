#include <ctype.h>

#include "interpret.h"
#include "Plural.h"
#include "PluralStack.h"
#include "CharStack.h"

void next(Plural_expr* e) {
	// ����״̬��
	// ע��token����ʱ��������a+bi����������зֳ�'a','+','bi'�����ɺ���ջ�㷨����ϲ����
	while (isspace(*e->expStr)) { // �Զ�ȥ���ո�
		(e->expStr)++;
	}
	if (*e->expStr == '\0') {
		if (e->token.type == PLURAL_TOKEN_OPEN ||
			e->token.type == PLURAL_TOKEN_ADD || e->token.type == PLURAL_TOKEN_SUB ||
			e->token.type == PLURAL_TOKEN_MUL || e->token.type == PLURAL_TOKEN_DIV) {
			// ��Ϊǰ���Ż�������ӽ�����˵����ʽûд��
			e->errType = PLURAL_ERROR_UNEXPECTED_ENDING;
		}
		else if (e->open_cnt != 0) {
			// ˵���������Ų�ƥ��
			e->errType = PLURAL_ERROR_NO_MATCHED_PARENTHESE;
		}
		else {
			e->token.type = PLURAL_TOKEN_END;
		}
	}
	else if (*e->expStr == '(') {
		if (e->token.type == PLURAL_TOKEN_IM || e->token.type == PLURAL_TOKEN_RE ||
			e->token.type == PLURAL_TOKEN_CLOSE) {
			//��Ϊ���ֻ������ţ�û����������ӣ�����
			e->errType = PLURAL_ERROR_UNEXPECTED_OPEN;
		}
		else {
			e->token.type = PLURAL_TOKEN_OPEN;
			e->open_cnt++; // δ��ƥ�����������+1
			(e->expStr)++;
		}

	}
	else if (*e->expStr == ')') {
		if (e->token.type == PLURAL_TOKEN_OPEN ||
			e->token.type == PLURAL_TOKEN_ADD || e->token.type == PLURAL_TOKEN_SUB ||
			e->token.type == PLURAL_TOKEN_MUL || e->token.type == PLURAL_TOKEN_DIV) {
			e->errType = PLURAL_ERROR_UNEXPECTED_CLOSE; // ������λ�ô���
		}
		else if (e->open_cnt == 0) {
			e->errType = PLURAL_ERROR_NO_MATCHED_PARENTHESE; // �����ű������Ŷ࣬ƥ�����
		}
		else {
			e->token.type = PLURAL_TOKEN_CLOSE;
			e->open_cnt--;
			(e->expStr)++;
		}
	}
	else if (*e->expStr == '*') {
		if (e->token.type == PLURAL_TOKEN_OPEN ||
			e->token.type == PLURAL_TOKEN_ADD || e->token.type == PLURAL_TOKEN_SUB ||
			e->token.type == PLURAL_TOKEN_MUL || e->token.type == PLURAL_TOKEN_DIV) {
			e->errType = PLURAL_ERROR_UNEXPECTED_OPERATOR; // �����λ�ô���
		}
		else {
			e->token.type = PLURAL_TOKEN_MUL;
			(e->expStr)++;
		}

	}
	else if (*e->expStr == '/') {
		if (e->token.type == PLURAL_TOKEN_OPEN ||
			e->token.type == PLURAL_TOKEN_ADD || e->token.type == PLURAL_TOKEN_SUB ||
			e->token.type == PLURAL_TOKEN_MUL || e->token.type == PLURAL_TOKEN_DIV) {
			e->errType = PLURAL_ERROR_UNEXPECTED_OPERATOR; // �����λ�ô���
		}
		else {
			e->token.type = PLURAL_TOKEN_DIV;
			(e->expStr)++;
		}
	}
	else if (*e->expStr == 'i' || *e->expStr == 'I') {
		if (e->token.type == PLURAL_TOKEN_IM || e->token.type == PLURAL_TOKEN_RE ||
			e->token.type == PLURAL_TOKEN_CLOSE) {
			e->errType = PLURAL_ERROR_UNEXPECTED_NUMBER; // ������λ�ô���
		}
		else {
			e->token.type = PLURAL_TOKEN_IM;
			e->token.value = 1;
			(e->expStr)++;
		}
	}
	else if (*e->expStr == '+' || *e->expStr == '-' || isdigit(*e->expStr)) {
		if ((*e->expStr == '+' || *e->expStr == '-') && // ��Ϊ+-���ܱ�ʾ��������Ҫ������һ���ж��߼�
			(e->token.type == PLURAL_TOKEN_RE || e->token.type == PLURAL_TOKEN_IM ||
				e->token.type == PLURAL_TOKEN_CLOSE)) { // ���ǰ�������ֻ������ţ�����Ϊ�������
			switch (*e->expStr) {
			case '+':
				e->token.type = PLURAL_TOKEN_ADD;
				break;
			case '-':
				e->token.type = PLURAL_TOKEN_SUB;
				break;
			}
			(e->expStr)++;
		}
		else {
			if (e->token.type == PLURAL_TOKEN_IM || e->token.type == PLURAL_TOKEN_RE ||
				e->token.type == PLURAL_TOKEN_CLOSE) { // ��Ϊ�����������ж�������һ��if���߼��ظ���
				e->errType = PLURAL_ERROR_UNEXPECTED_NUMBER;
			}
			else {
				int minus = 1; // ��������
				while (*e->expStr == '+' || *e->expStr == '-') { // ������������ǰ�������ţ�������ȡ��
					if (*e->expStr == '-') {
						minus *= -1;
					}
					(e->expStr)++;
				}
				bool is_float = false; // �ж����������ֻ���С������
				int decimal_e = 1; // С��λ������¼
				e->token.value = *e->expStr - '0';
				(e->expStr)++;
				while (isdigit(*e->expStr) || *e->expStr == '.') {
					if (*e->expStr == '.') {
						is_float = true;
						(e->expStr)++;
						continue;
					}
					if (!is_float) {
						e->token.value = e->token.value * 10 + (*e->expStr - '0');
						(e->expStr)++;
					}
					else {
						decimal_e *= 10;
						e->token.value += (*e->expStr - '0') * 1.0 / decimal_e;
						(e->expStr)++;
					}
				}
				e->token.value *= minus;
				if (*e->expStr == 'i' || *e->expStr == 'I') {
					e->token.type = PLURAL_TOKEN_IM; // ���ĩβ��i����Ϊ�Ǹ���
					(e->expStr)++;
				}
				else { // ��Ϊ��ʵ��
					e->token.type = PLURAL_TOKEN_RE;
				}
			}
		}
	}
	else {
		e->errType = PLURAL_ERROR_UNKNOWN_TOKEN;
	}
}

char precede(char c1, char c2)
{ // ջ����������ȼ��ж�
	int c_temp1, c_temp2;
	switch (c1) {
	case '*':
	case '/':
		c_temp1 = 4;
		break;
	case '+':
	case '-':
		c_temp1 = 2;
		break;
	case '(':
		c_temp1 = 0;
		break;
	case ')':
		c_temp1 = 5;
		break;
	case '#':
		c_temp1 = -1;
	}
	switch (c2) {
	case '*':
	case '/':
		c_temp2 = 3;
		break;
	case '+':
	case '-':
		c_temp2 = 1;
		break;
	case '(':
		c_temp2 = 5;
		break;
	case ')':
		c_temp2 = 0;
		break;
	case '#':
		c_temp2 = -1;
	}
	if (c_temp1 < c_temp2)
		return('<');
	else if (c_temp1 == c_temp2)
		return('=');
	else
		return('>');
}

Plural operate(Plural a, char theta, Plural b)
{ // ������������
	switch (theta) {
	case '+':
		return Plural_add(a, b);
	case '-':
		return Plural_sub(a, b);
	case '*':
		return Plural_mul(a, b);
	case '/':
		return Plural_div(a, b);
	}
}

Plural plural_eval(const char* expStr, int* errType)
{ // TODO: ��ȡtoken���߼���ջ�㷨�߼�������һ���¿ɶ��Ա�ͣ����ع�
    char oper;
	Plural_expr e;
	e.expStr = expStr;
	e.token.type = PLURAL_TOKEN_INIT;
	e.errType = PLURAL_ERROR_NONE;
	e.open_cnt = 0;
	PluralStack *ps = PluralStack_init();
	CharStack *cs = CharStack_init();
	CharStack_push(cs, '#');
	next(&e);
	while (1) {
		if (e.errType) { // �����ظ�
			*errType = e.errType;
			Plural p;
			p.real = -1;
			p.imagin = -1;
			return p;
		}
		// ����token����ȡ��ͬ����
		oper = 0;
		Plural p = Plural_init();
		switch (e.token.type) {
		// case PLURAL_TOKEN_INIT:
		case PLURAL_TOKEN_RE:
			p.real = e.token.value;
			break;
		case PLURAL_TOKEN_IM:
			p.imagin = e.token.value;
			break;
		case PLURAL_TOKEN_ADD:
			oper = '+';
			break;
		case PLURAL_TOKEN_SUB:
			oper = '-';
			break;
		case PLURAL_TOKEN_MUL:
			oper = '*';
			break;
		case PLURAL_TOKEN_DIV:
			oper = '/';
			break;
		case PLURAL_TOKEN_OPEN:
			oper = '(';
			break;
		case PLURAL_TOKEN_CLOSE:
			oper = ')';
			break;
		case PLURAL_TOKEN_END:
		    oper = '#';
		    break;
		}

		if (oper == 0) { // �ǲ�����
            PluralStack_push(ps, p);
            next(&e);
			if (e.errType) { // �����ظ�
				*errType = e.errType;
				Plural p;
				p.real = -1;
				p.imagin = -1;
				return p;
			}
        }
        else { // �ǲ�����
            char cmp = precede(CharStack_top(cs), oper);
            while (cmp == '>') {
                char theta = CharStack_top(cs);
                CharStack_pop(cs);
                Plural b = PluralStack_top(ps);
                PluralStack_pop(ps);
                Plural a = PluralStack_top(ps);
                PluralStack_pop(ps);
                PluralStack_push(ps, operate(a, theta, b));
                cmp = precede(CharStack_top(cs), oper);
            }
            switch (cmp) {
            case '<':
                CharStack_push(cs, oper);
                break;
            case '=':
                CharStack_pop(cs);
                break;
            }
            if (oper != '#') {
                next(&e);
				if (e.errType) { // �����ظ�
					*errType = e.errType;
					Plural p;
					p.real = -1;
					p.imagin = -1;
					return p;
				}
            } else {
                break;
            }
        }
	}
	return PluralStack_top(ps);
}
