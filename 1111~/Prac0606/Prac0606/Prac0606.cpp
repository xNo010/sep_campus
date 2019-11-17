#include <iostream>

int main()
{
	int32_t kuku[9][9] =
	{
		{0, },
	};

	// 配列kukuに値を代入する
	for (int32_t i = 0; i < 9; i++)
	{
		for (int32_t j = 0; j < 9; j++)
		{
			kuku[i][j] = (i + 1) * (j + 1);
		}
	}

	for (int32_t i = 0; i < 9; i++)
	{
		for (int32_t j = 0; j < 9; j++)
		{
			printf("%2d ", kuku[i][j]);
		}
		printf("\n");
	}
}