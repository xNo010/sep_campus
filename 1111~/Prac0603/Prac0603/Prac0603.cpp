#include <iostream>

int main()
{
	int32_t temp[10] = { 0, };

	for (int32_t i = 0; i < 10; i++)
	{
		std::cout << "入力:";
		std::cin >> temp[i];
	}

	printf("偶数:");
	for (int32_t i = 0; i < 10; i++)
	{
		if (temp[i] % 2 != 0)
			continue;

		printf("%d ", temp[i]);
	}
	printf("\n奇数:");
	for (int32_t i = 0; i < 10; i++)
	{
		if (temp[i] % 2 == 0)
			continue;

		printf("%d ", temp[i]);
	}
}