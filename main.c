#include <stdio.h>
#include <string.h>
#include "interpret.h"
#include "Plural.h"

#define MAXSIZE 500

int main()
{
	char buf[MAXSIZE];
	int errType;
	Plural p;
	printf("**************************************************\n");
	printf("*              Plural Calculator                 *\n");
	printf("*           By MIS 1801, Haowei Li, CSU          *\n");
	printf("**************************************************\n\n");
	printf("ʾ����\n");
	printf(">>> (1+3i)/3.2-(1*2-1/(3I+2))\n");
	printf("-1.53+0.71i\n\n");
	printf("����\"help\"�Ի�ȡ���������Ϣ������\"exit\"���˳�����\n");
	printf(">>> ");
	while (fgets(buf, MAXSIZE, stdin)) {
		if (strcmp(buf, "exit\n") == 0) {
			return 0;
		}
		else if (strcmp(buf, "help\n") == 0) {
            printf("������������֧�ִ��������ȼ��ĸ����������㣬�ɱ����ܵĺϷ������У�+ - * / ( ) i��\n");
            printf("��������ͳһ��Բ���ű�ʾ����\"*\"��ʾ�˷�����\"/\"��ʾ�������������ֺ��\"i\"��\"I\"��ʾ�ò���Ϊ������\n");
            printf("������Ĭ�ϱ�����λС��������\"exit\"���˳�����\n");
            printf("ʾ����\n");
            printf(">>> (1+3i)/3.2-(1*2-1/(3I+2))\n");
            printf("-1.53+0.71i\n");
			printf("ע���ڴ�����λ���������ʱ��������ܳ��ֲ���Ԥ֪�Ĵ���\n\n");
			printf("���ߣ��Ź�1801���쿣�GitHub: Catigeart\n");
            printf("ףʹ����죡\n\n");
		}
		else {
            errType = -1;
            p = plural_eval(buf, &errType);           
            if (errType != -1) {
				switch (errType) {
				case PLURAL_ERROR_UNEXPECTED_POINT:
					printf("����С��������λ�ò��Ϸ�\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_ENDING:
					printf("���󣺱��ʽ������\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_OPEN:
					printf("��������������λ�ò��Ϸ�\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_CLOSE:
					printf("��������������λ�ò��Ϸ�\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_NUMBER:
					printf("��������������λ�ò��Ϸ�\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_OPERATOR:
					printf("�������������λ�ò��Ϸ�\n");
					break;
				case PLURAL_ERROR_NO_MATCHED_PARENTHESE:
					printf("�����������������Ų�ƥ��\n");
					break;
				case PLURAL_ERROR_UNKNOWN_TOKEN:
					printf("����δ֪����\n");
					break;
				}
            }
			else {
				Plural_print(p);
			}
		}
		printf(">>> ");
	}
	return 0;
}
