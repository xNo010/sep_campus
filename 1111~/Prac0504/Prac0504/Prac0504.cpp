#include <iostream>

int main()
{
	int32_t a = 1;

	for (int32_t i = 7; i > 0; i--)
	{
		a *= i;
	}
	printf("7の階乗:%d\n", a);
}