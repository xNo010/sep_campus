#include <iostream>

int main()
{
	int32_t a = -1;
	int32_t temp = 0;
	int32_t InputNumCount = 0;

	printf("0を入力すると、それまで入力した数値の平均値を表示します\n");

	while (a != 0)
	{
		std::cout << "数値入力:";
		std::cin >> a;

		temp += a;
		if (a != 0)
		{
			InputNumCount++;
		}
	}

	printf("平均値:%d\n", temp / InputNumCount);

}