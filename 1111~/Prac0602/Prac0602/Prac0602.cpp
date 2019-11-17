#include <iostream>
#include <time.h>

#define MaxNum 10

int main()
{
	int32_t Num[MaxNum] = { 0, };

	printf("入力された順番と逆の順番で表示し直します\n");

	for (int32_t i = 0; i < MaxNum; i++)
	{
		std::cout << i + 1 << "番目の数値入力:";
		std::cin >> Num[i];

		if (i == MaxNum - 1)
		{
			for (int32_t j = MaxNum - 1; j >= 0; j--)
			{
				printf("%d ", Num[j]);
			}
			printf("\n");
		}
	}
}