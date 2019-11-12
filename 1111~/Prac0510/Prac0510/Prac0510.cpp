#include <iostream>

int main()
{
	int32_t temp = 0;

	printf("数値分*を表示します\n");
	std::cout << "入力:";
	std::cin >> temp;

	for (int32_t i = 0; i < temp; i++)
	{
		printf("*");
	}
}