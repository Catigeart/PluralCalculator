#pragma once

#include <stdio.h>
#include "Plural.h"

enum { // token◊¥Ã¨
	PLURAL_TOKEN_INIT,
	PLURAL_TOKEN_RE, // real
	PLURAL_TOKEN_IM, // imaginary
	PLURAL_TOKEN_ADD,
	PLURAL_TOKEN_SUB,
	PLURAL_TOKEN_MUL,
	PLURAL_TOKEN_DIV,
	PLURAL_TOKEN_OPEN,
	PLURAL_TOKEN_CLOSE,
	PLURAL_TOKEN_END
};

enum { // ¥ÌŒÛ¿‡–Õ
	PLURAL_ERROR_NONE,
	PLURAL_ERROR_UNEXPECTED_POINT,
	PLURAL_ERROR_UNEXPECTED_ENDING,
	PLURAL_ERROR_UNEXPECTED_OPEN,
	PLURAL_ERROR_UNEXPECTED_CLOSE,
	PLURAL_ERROR_UNEXPECTED_NUMBER,
	PLURAL_ERROR_UNEXPECTED_OPERATOR,
	PLURAL_ERROR_NO_MATCHED_PARENTHESE,
	PLURAL_ERROR_UNKNOWN_TOKEN
};

typedef enum { false, true } bool;

typedef struct {
	int type;
	double value;
} Plural_token;

typedef struct {
	const char* expStr;
	Plural_token token;
	int errType;
	int open_cnt;
} Plural_expr;


void next(Plural_expr* e);
char precede(char c1, char c2);
Plural operate(Plural a, char theta, Plural b);
Plural plural_eval(const char* expStr, int* errType);
