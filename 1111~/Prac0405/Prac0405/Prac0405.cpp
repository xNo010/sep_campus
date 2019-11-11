#include <iostream>

int main()
{
	int32_t Int01;

	printf("偶数or奇数の判定\n");

	std::cout << "整数値入力:";
	std::cin >> Int01;

	if (Int01 % 2 == 0)
	{
		printf("%dは偶数です\n", Int01);
	}
	else if (Int01 % 2 == 1)
	{
		printf("%dは奇数です\n", Int01);
	}
	else
	{
		printf("%dは異常な値です\n", Int01);
	}
}