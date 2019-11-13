#include <iostream>

int main()
{
	int32_t temp = 0;

	printf("数値を入力し合計100を超えたら終了します\n");

	while (temp <= 100)
	{
		int32_t Value;

		std::cout << "数値入力:";
		std::cin >> Value;

		temp += Value;
	}

	printf("%d", temp);
}