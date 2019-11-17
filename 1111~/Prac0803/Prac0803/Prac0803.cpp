#include <iostream>

int main()
{
	int32_t Value[10] = { 0, };

	printf("10個の数値を入力\n");
	for (int32_t i = 0; i < 10; i++)
	{
		scanf_s("%d", &Value[i]);
	}

	// ここまでやる必要なし
	//int32_t save = 0;
	//for (int32_t i = 0; i < 10 / 2; i++)
	//{
	//	save = *(Value + i);
	//	*(Value + i) = *(Value + 10 - i - 1);
	//	*(Value + 10 - i - 1) = save;
	//}
	//
	//for (int32_t i = 0; i < 10; i++)
	//{
	//	printf("%d ", *(Value + i));
;	//}

	for (int32_t i = 10 - 1; i >= 0; i--)
	{
		printf("%d ", *(Value + i));
	}
}