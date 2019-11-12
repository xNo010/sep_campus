#include <iostream>

int main()
{
	int32_t temp = 0;
	int32_t OverCount = 0;

	printf("入力した数値分、0～9の数値を表示します\n");
	std::cout << "入力:";
	std::cin >> temp;

	for (int32_t i = 0; i < temp; i++)
	{
		int32_t Save = i;
		if (i != 0 && Save % 10 == 0)
		{
			OverCount++;
		}
		Save = (-10 * OverCount) + i;
		printf("%d ", Save);
	}
}