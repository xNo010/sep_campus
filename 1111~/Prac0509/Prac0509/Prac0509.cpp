#include <iostream>

int main()
{
	int32_t temp = 0;
	int32_t Max = 0;
	int32_t Min = 0;

	printf("10個の入力した整数から最大値と最小値を求めます\n");

	for (int32_t i = 0; i < 10; i++)
	{
		std::cout << "整数値入力:";
		std::cin >> temp;

		if (i == 0)
		{
			Max = Min = temp;
		}
		else
		{
			if (temp > Max)
			{
				Max = temp;
			}
			if (temp < Min)
			{
				Min = temp;
			}
		}
	}

	printf("最小値:%d, 最大値:%d", Min, Max);
}