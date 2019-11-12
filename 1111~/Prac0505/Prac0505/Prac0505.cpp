#include <iostream>

int main()
{
	int32_t Int[10] = { 0, };
	int32_t temp = 0;

	printf("10個の整数値から平均値を求めます\n");
	std::cout << "10個の整数を入力:";

	for (int32_t i = 0; i < 10; i++)
	{
		std::cin >> Int[i];
		temp += Int[i];
	}
	printf("合計値:%d, 平均値%d", temp, temp / 10);
}