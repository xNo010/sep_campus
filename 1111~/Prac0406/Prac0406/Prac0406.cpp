#include <iostream>

int main()
{
	int32_t Int01;

	std::cout << "整数値入力:";
	std::cin >> Int01;

	if (Int01 >= 0)
	{
		if (Int01 % 2 == 0)
		{
			printf("正の偶数\n");
		}
		else
		{
			printf("正の奇数\n");
		}
	}
	else 
	{
		if (Int01 % 2 == 0)
		{
			printf("負の偶数\n");
		}
		else
		{
			printf("負の奇数\n");
		}
	}
}