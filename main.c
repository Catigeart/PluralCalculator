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
	printf("示例：\n");
	printf(">>> (1+3i)/3.2-(1*2-1/(3I+2))\n");
	printf("-1.53+0.71i\n\n");
	printf("输入\"help\"以获取更多帮助信息，输入\"exit\"以退出程序。\n");
	printf(">>> ");
	while (fgets(buf, MAXSIZE, stdin)) {
		if (strcmp(buf, "exit\n") == 0) {
			return 0;
		}
		else if (strcmp(buf, "help\n") == 0) {
            printf("本复数计算器支持带括号优先级的复数四则运算，可被接受的合法符号有：+ - * / ( ) i；\n");
            printf("其中括号统一以圆括号表示，以\"*\"表示乘法，以\"/\"表示除法，输入数字后接\"i\"或\"I\"表示该部分为复数；\n");
            printf("计算结果默认保留两位小数，输入\"exit\"以退出程序。\n");
            printf("示例：\n");
            printf(">>> (1+3i)/3.2-(1*2-1/(3I+2))\n");
            printf("-1.53+0.71i\n");
			printf("注：在处理数位极大的运算时，运算可能出现不可预知的错误。\n\n");
			printf("作者：信管1801李昊炜，GitHub: Catigeart\n");
            printf("祝使用愉快！\n\n");
		}
		else {
            errType = -1;
            p = plural_eval(buf, &errType);           
            if (errType != -1) {
				switch (errType) {
				case PLURAL_ERROR_UNEXPECTED_POINT:
					printf("错误：小数点输入位置不合法\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_ENDING:
					printf("错误：表达式不完整\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_OPEN:
					printf("错误：左括号输入位置不合法\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_CLOSE:
					printf("错误：右括号输入位置不合法\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_NUMBER:
					printf("错误：运算数输入位置不合法\n");
					break;
				case PLURAL_ERROR_UNEXPECTED_OPERATOR:
					printf("错误：运算符输入位置不合法\n");
					break;
				case PLURAL_ERROR_NO_MATCHED_PARENTHESE:
					printf("错误：左括号与右括号不匹配\n");
					break;
				case PLURAL_ERROR_UNKNOWN_TOKEN:
					printf("错误：未知输入\n");
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
