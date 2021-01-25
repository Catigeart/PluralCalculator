#pragma once

#include <string.h>

typedef struct {
	double real;
	double imagin;
} Plural;

Plural Plural_init();
Plural Plural_add(Plural p1, Plural p2);
Plural Plural_sub(Plural p1, Plural p2);
Plural Plural_mul(Plural p1, Plural p2);
Plural Plural_div(Plural p1, Plural p2);
void Plural_print(Plural p);