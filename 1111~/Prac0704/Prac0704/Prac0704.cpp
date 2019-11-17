#include <iostream>

int main()
{
	char	sbinary[17] = "";
	int32_t	value;

	scanf_s("%d", &value);

	// 配列 sbinaryに'0'か'1'を代入する
	for (int32_t i = 16 - 1; i >= 0; i--)
	{
		// 0と1　文字コードは一つ違い
		sbinary[i] = '0' + (value % 2);
		value /= 2;

		//if (value % 2 == 0)
		//{
		//	sbinary[i] = '0';
		//	value /= 2;
		//}
		//else
		//{
		//	sbinary[i] = '1';
		//	value /= 2;
		//}
	}

	printf("%s", sbinary);
}