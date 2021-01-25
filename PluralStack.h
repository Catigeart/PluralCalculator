#pragma once

#include <stdlib.h>
#include "Plural.h"
#include "Stackdef.h"

#define STACK(n) PluralStack_##n
#define STACKNODE PluralStack
#define STACKDATA Plural

typedef struct STACKNODE {
	STACKDATA data;
    struct STACKNODE* next;
}STACKNODE;

STACKNODE* STACK(init)();
void STACK(push)(STACKNODE* top, STACKDATA x);
void STACK(pop)(STACKNODE* top);
STACKDATA STACK(top)(STACKNODE* top);
int STACK(empty)(STACKNODE* top);
void STACK(clear)(STACKNODE* top);
void STACK(exception)(int err_type);



