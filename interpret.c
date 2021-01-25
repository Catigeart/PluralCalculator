#include <ctype.h>

#include "interpret.h"
#include "Plural.h"
#include "PluralStack.h"
#include "CharStack.h"

void next(Plural_expr* e) {
	// 简易状态机
	// 注：token处理时，对形如a+bi的情况，将切分成'a','+','bi'处理，由后续栈算法运算合并结果
	while (isspace(*e->expStr)) { // 自动去除空格
		(e->expStr)++;
	}
	if (*e->expStr == '\0') {
		if (e->token.type == PLURAL_TOKEN_OPEN ||
			e->token.type == PLURAL_TOKEN_ADD || e->token.type == PLURAL_TOKEN_SUB ||
			e->token.type == PLURAL_TOKEN_MUL || e->token.type == PLURAL_TOKEN_DIV) {
			// 若为前括号或运算符接结束，说明算式没写完
			e->errType = PLURAL_ERROR_UNEXPECTED_ENDING;
		}
		else if (e->open_cnt != 0) {
			// 说明左右括号不匹配
			e->errType = PLURAL_ERROR_NO_MATCHED_PARENTHESE;
		}
		else {
			e->token.type = PLURAL_TOKEN_END;
		}
	}
	else if (*e->expStr == '(') {
		if (e->token.type == PLURAL_TOKEN_IM || e->token.type == PLURAL_TOKEN_RE ||
			e->token.type == PLURAL_TOKEN_CLOSE) {
			//若为数字或右括号，没有运算符连接，报错
			e->errType = PLURAL_ERROR_UNEXPECTED_OPEN;
		}
		else {
			e->token.type = PLURAL_TOKEN_OPEN;
			e->open_cnt++; // 未被匹配的左括号数+1
			(e->expStr)++;
		}

	}
	else if (*e->expStr == ')') {
		if (e->token.type == PLURAL_TOKEN_OPEN ||
			e->token.type == PLURAL_TOKEN_ADD || e->token.type == PLURAL_TOKEN_SUB ||
			e->token.type == PLURAL_TOKEN_MUL || e->token.type == PLURAL_TOKEN_DIV) {
			e->errType = PLURAL_ERROR_UNEXPECTED_CLOSE; // 右括号位置错误
		}
		else if (e->open_cnt == 0) {
			e->errType = PLURAL_ERROR_NO_MATCHED_PARENTHESE; // 右括号比左括号多，匹配错误
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
			e->errType = PLURAL_ERROR_UNEXPECTED_OPERATOR; // 运算符位置错误
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
			e->errType = PLURAL_ERROR_UNEXPECTED_OPERATOR; // 运算符位置错误
		}
		else {
			e->token.type = PLURAL_TOKEN_DIV;
			(e->expStr)++;
		}
	}
	else if (*e->expStr == 'i' || *e->expStr == 'I') {
		if (e->token.type == PLURAL_TOKEN_IM || e->token.type == PLURAL_TOKEN_RE ||
			e->token.type == PLURAL_TOKEN_CLOSE) {
			e->errType = PLURAL_ERROR_UNEXPECTED_NUMBER; // 运算数位置错误
		}
		else {
			e->token.type = PLURAL_TOKEN_IM;
			e->token.value = 1;
			(e->expStr)++;
		}
	}
	else if (*e->expStr == '+' || *e->expStr == '-' || isdigit(*e->expStr)) {
		if ((*e->expStr == '+' || *e->expStr == '-') && // 因为+-可能表示正负，需要和数字一起判断逻辑
			(e->token.type == PLURAL_TOKEN_RE || e->token.type == PLURAL_TOKEN_IM ||
				e->token.type == PLURAL_TOKEN_CLOSE)) { // 如果前面是数字或右括号，则认为是运算符
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
				e->token.type == PLURAL_TOKEN_CLOSE) { // 认为是运算数的判定，与上一个if的逻辑重复了
				e->errType = PLURAL_ERROR_UNEXPECTED_NUMBER;
			}
			else {
				int minus = 1; // 正负因子
				while (*e->expStr == '+' || *e->expStr == '-') { // 允许连续输入前导正负号，遇符号取反
					if (*e->expStr == '-') {
						minus *= -1;
					}
					(e->expStr)++;
				}
				bool is_float = false; // 判断是整数部分还是小数部分
				int decimal_e = 1; // 小数位基数记录
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
					e->token.type = PLURAL_TOKEN_IM; // 如果末尾接i，认为是复数
					(e->expStr)++;
				}
				else { // 认为是实数
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
{ // 栈内外算符优先级判断
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
{ // 复数四则运算
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
{ // TODO: 读取token的逻辑和栈算法逻辑杂糅在一起导致可读性变低，待重构
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
		if (e.errType) { // 代码重复
			*errType = e.errType;
			Plural p;
			p.real = -1;
			p.imagin = -1;
			return p;
		}
		// 分析token并采取不同动作
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

		if (oper == 0) { // 是操作数
            PluralStack_push(ps, p);
            next(&e);
			if (e.errType) { // 代码重复
				*errType = e.errType;
				Plural p;
				p.real = -1;
				p.imagin = -1;
				return p;
			}
        }
        else { // 是操作符
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
				if (e.errType) { // 代码重复
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
