#include <iostream>

void ExchangeValue(int32_t* pChangedVal, int32_t* ConvVal)
{
	int32_t temp;
	temp = *pChangedVal;
	*pChangedVal = *ConvVal;
	*ConvVal = temp;
}

int main()
{
	int32_t value[10] = { 0, };
	printf("10個の数値を入力し、それを小さい順に並べる\n");
	for (int32_t i = 0; i < 10; i++)
	{
		scanf_s("%d", &value[i]);
	}

	for (int32_t i = 0; i < 10 - 1; i++)
	{
		for (int32_t j = i + 1; j < 10; j++)
		{
			if (*(value + i) >= *(value + j))
			{
				ExchangeValue((value + i), (value + j));
			}
		}
	}

	for (int32_t i = 0; i < 10; i++)
	{
		printf("%d ", value[i]);
	}
}