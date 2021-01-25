#include "PluralStack.h"

// 伪面向对象编程
// 宏重复定义以实现伪泛型

STACKNODE* STACK(init)()
{
	STACKNODE* top = (STACKNODE*)malloc(sizeof(STACKNODE));
	if (top == NULL) {
		STACK(exception)(STACK_ERROR_MALLOC_FAILED);
	}
	else {
		top->next = NULL;
	}	
	return top;
}

void STACK(push)(STACKNODE* top, STACKDATA x)
{
	STACKNODE* p;
	p = (STACKNODE*)malloc(sizeof(STACKNODE));
	if (p == NULL) {
		STACK(exception)(STACK_ERROR_MALLOC_FAILED);
	}
	else {
		p->data = x;
		p->next = top->next;
		top->next = p;
	}	
}

void STACK(pop)(STACKNODE* top)
{
	STACKDATA x;
	STACKNODE* p;
	if (STACK(empty)(top))
		return;
	p = top->next;
	top->next = p->next;
	free(p);
	return;
}

STACKDATA STACK(top)(STACKNODE* top)
{
	STACKDATA x;
	if (STACK(empty)(top)) {
		x.real = 0;
		x.imagin = 0;
		STACK(exception)(STACK_ERROR_EXPECT_NOT_EMPTY);
	}
	else
		x = top->next->data;
	return x;
}

int STACK(empty)(STACKNODE* top)
{
	if (top->next == NULL)
		return 1;
	else
		return 0;
}

void STACK(clear)(STACKNODE* top)
{
	while (!STACK(empty)(top))
		STACK(pop)(top);
}

void STACK(exception)(int err_type)
{
	switch (err_type) {
	case STACK_ERROR_NONE:
		break;
	case STACK_ERROR_EXPECT_NOT_EMPTY:
		printf("\nERROR: STACK_ERROR_EXPECT_NOT_EMPTY\n\n");
		break;
	case STACK_ERROR_MALLOC_FAILED:
		printf("\nERROR: STACK_ERROR_MALLOC_FAILED\n\n");
		break;
	}
}