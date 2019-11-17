#include <iostream>

int main()
{
	int32_t a = -1;
	int32_t temp = 0;

	printf("0が入力されるまで入力した数値を足します\n");

	while (a != 0)
	{
		std::cout << "数値入力:";
		std::cin >> a;

		temp += a;
	}

	printf("合計値:%d\n", temp);
}