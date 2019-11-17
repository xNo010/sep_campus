#include <iostream>

int main()
{
	int32_t IntValue[10];

	for (int32_t i = 0; i < 10; i++)
	{
		std::cout << "数値入力:";
		std::cin >> IntValue[i];
	}

	printf("昇順にソートします\n");

	int32_t temp = 0;
	for (int32_t i = 0; i < 10 - 1; i++)
	{
		for (int32_t j = i + 1; j < 10; j++)
		{
			if (IntValue[i] >= IntValue[j])
			{
				temp = IntValue[i];
				IntValue[i] = IntValue[j];
				IntValue[j] = temp;
			}
		}
	}

	for (int32_t i = 0; i < 10; i++)
	{
		printf("%d ", IntValue[i]);
	}
}