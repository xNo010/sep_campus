#include <iostream>

int main()
{
	int32_t value = 0;
	int32_t binary[16] = { 0, };
	int32_t temp = 0;

	scanf_s("%d", &value);
	
	// 配列binaryに0か1を代入する
	for (int32_t i = 16 - 1; i >= 0; i--)
	{
		if (value != 0)
		{
			binary[i] = value % 2;
			value /= 2;
		}
		else
		{
			binary[i] = 0;
		}
	}

	for (int32_t i = 0; i < 16; i++)
	{
		printf("%d", binary[i]);
	}
}