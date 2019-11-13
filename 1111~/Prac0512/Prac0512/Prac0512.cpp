#include <iostream>

int main()
{
	int32_t temp = 1;

	printf("10000より小さい3の累乗を表示\n");

	while (temp <= 10000)
	{
		temp *= 3;
		if (temp <= 10000)
		{
			printf("%d\n", temp);
		}
	}
}