#include "stdio.h"
#include "Plural.h"

// 伪面向对象编程

Plural Plural_init()
{
	Plural p;
	p.real = 0;
	p.imagin = 0;
	return p;
}

Plural Plural_add(Plural p1, Plural p2)
{
	p1.real = p1.real + p2.real;
	p1.imagin = p1.imagin + p2.imagin;
	return p1;
}

Plural Plural_sub(Plural p1, Plural p2)
{
	p1.real = p1.real - p2.real;
	p1.imagin = p1.imagin - p2.imagin;
	return p1;
}

Plural Plural_mul(Plural p1, Plural p2)
{
	Plural p;
	p.real = p1.real * p2.real - p1.imagin * p2.imagin;
	p.imagin = p1.imagin * p2.real + p1.real * p2.imagin;
	return p;
}

Plural Plural_div(Plural p1, Plural p2)
{
	Plural p;
	p.real = (p1.real * p2.real + p1.imagin * p2.imagin) / (p2.real * p2.real + p2.imagin * p2.imagin);
	p.imagin = (p1.imagin * p2.real - p1.real * p2.imagin) / (p2.real * p2.real + p2.imagin * p2.imagin);
	return p;
}

void Plural_print(Plural p) {
	printf("%.2f", p.real);
	if (p.imagin >= 0) {
		printf("+");
	}
	printf("%.2fi\n", p.imagin);
}
