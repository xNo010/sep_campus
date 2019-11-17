#include <iostream>

int main()
{
	int32_t InputNum = 0;

	printf("練習問題5-22\n");

	std::cout << "サイズ入力:";
	std::cin >> InputNum;

	for (int32_t i = 0; i < InputNum; i++)
	{
		for (int32_t j = 0; j < InputNum; j++)
		{
			//if ((i == 0 || i == InputNum - 1) &&
			//	(j == 0 || j == InputNum - 1))
			if (i == j || InputNum - i - 1 == j)
			{
				printf("X");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}