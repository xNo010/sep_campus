#include <iostream>

int main()
{
	int x;
	int* p;

	p = &x;

	*p = 12;

	printf("%d\n", x);
}