#include <iostream>

int main()
{
	int32_t x, y;

	x = 5;
	y = 20;

	printf("元の値:x = %d, y = %d\n", x, y);

	int32_t temp = x;
	x = y;
	y = temp;

	printf("値入れ替えました:x = %d, y = %d\n", x, y);
}
