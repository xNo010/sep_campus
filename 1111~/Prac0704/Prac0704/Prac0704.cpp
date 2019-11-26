#include <iostream>

int main()
{
	char	sbinary[17] = "";
	int32_t	value;

	printf("数値入力:");
	scanf_s("%d", &value);

	// 配列 sbinaryに'0'か'1'を代入する
	for (int32_t i = 16; i > 0; i--)
	{
		// 0と1　文字コードは一つ違い
		// 2で割っていって出た余りが2進数の数値
		sbinary[i - 1] = '0' + (value % 2);
		value /= 2;
	}

	printf("%s", sbinary);
}