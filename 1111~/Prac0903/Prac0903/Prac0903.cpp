#include <iostream>

int32_t IsBigger(int32_t value1, int32_t value2)
{
	if (value1 > value2)
	{
		return value1;
	}
	else
	{
		return value2;
	}
	return 0;
}

int main()
{
	int32_t x, y, z;

	printf("3個の数値入力\n");
	scanf_s("%d", &x);
	scanf_s("%d", &y);
	scanf_s("%d", &z);

	printf("一番大きい値:%d\n", IsBigger(IsBigger(x, y), z));
}