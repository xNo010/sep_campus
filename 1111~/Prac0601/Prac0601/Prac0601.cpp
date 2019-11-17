#include <iostream>

int main()
{
	int32_t InputNum[10] = { 0, };

	printf("入力した数値を二倍したものを表示します\n");

	for (int32_t i = 0; i < 10; i++)
	{
		// scanf_s("%d番目の数値入力:%d", &i, &InputNum[i]);
		std::cout << i + 1 << "番目の数値入力:";
		std::cin >> InputNum[i];

		if (i == 10 - 1)
		{
			for (i = 0; i < 10; i++)
			{
				printf("%d ", InputNum[i] * 2);
			}
		}
	}
}