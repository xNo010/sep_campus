#include <iostream>

int main()
{
	int32_t InputNum = 0;

	printf("入力したサイズの三角形を表示します\n");

	std::cout << "サイズ入力:";
	std::cin >> InputNum;

	int32_t temp = 1;
	while (InputNum >= temp)
	{
		for (int32_t i = 0; i < temp; i++)
		{
			printf("$");
		}
		printf("\n");
		temp++;
	}
}