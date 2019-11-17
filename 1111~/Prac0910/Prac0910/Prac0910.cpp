#include <iostream>

void AscendingOrder(int32_t* pValue, int32_t ElementCount)
{
	for (int32_t i = 0; i < ElementCount - 1; i++)
	{
		int32_t temp;
		for (int32_t j = i + 1; j < 10; j++)
		{
			if (*(pValue + i) >= *(pValue + j))
			{
				temp = *(pValue + i);
				*(pValue + i) = *(pValue + j);
				*(pValue + j) = temp;
			}
		}
	}
}

int main()
{
	int32_t Values[10] = { 0, };
	printf("10個の数値を入力し、それを小さい順に並べる\n");
	for (int32_t i = 0; i < 10; i++)
	{
		scanf_s("%d", &Values[i]);
	}

	AscendingOrder(Values, 10);

	for (int32_t i = 0; i < 10; i++)
	{
		printf("%d ", Values[i]);
	}
}