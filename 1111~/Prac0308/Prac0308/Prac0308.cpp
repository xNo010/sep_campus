#include <iostream>

int main()
{
	int32_t x;

	x = 50;

	printf("現在の値:%d\n", x);

	printf("先インクリメント:%d\n", ++x);
	x = 50;
	printf("後インクリメント:%d\n", x++);
	x = 50;
	printf("先デクリメント:%d\n", --x);
	x = 50;
	printf("後デクリメント:%d\n", x--);
}
