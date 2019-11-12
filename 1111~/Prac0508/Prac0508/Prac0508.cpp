#include <iostream>

int main()
{
	int32_t a = 0;
	int32_t temp = 0;

	printf("10個の自然数から最大値を求めます\n");
	for (int32_t i = 1; i <= 3; i++)
	{
		std::cout << i << "個目の自然数:";
		std::cin >> a;
		temp += a;
	}

	printf("最大値:%d\n", temp);
}