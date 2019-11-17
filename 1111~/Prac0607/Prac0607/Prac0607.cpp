#include <iostream>

int main()
{
	int32_t kuku[9][9];
	int32_t IntValue[2] = { 1, 1 };

	// 配列kukuに値を代入する
	for (int32_t i = 0; i < 9; i++)
	{
		for (int32_t j = 0; j < 9; j++)
		{
			kuku[i][j] = (i + 1) * (j + 1);
		}
	}

	printf("1桁の数値(1～9)を二つ入力して\n");

	int32_t Count = 0;

	while (Count != 2)
	{
		std::cout << Count + 1 << "番目の数値入力:";
		std::cin >> IntValue[Count];

		if (IntValue[Count] <= 0 || IntValue[Count] >= 10)
		{
			continue;
		}

		Count++;
	}

	printf("%2d\n", kuku[IntValue[0] - 1][IntValue[1] - 1]);
}