#include <iostream>
#include <Windows.h>

#define MaxSize 7

int main()
{
	int32_t Diamond[MaxSize][MaxSize];
	int32_t first, end;

	for (int32_t i = 0; i < MaxSize; i++)
	{
		for (int32_t j = 0; j < MaxSize; j++)
		{
			Diamond[i][j] = 0;
		}
	}

	first = 2;
	end =	4;

	while (first >= 0 || end < MaxSize)
	{
		for (int32_t i = 0; i < MaxSize; i++)
		{
			for (int32_t j = 0; j < MaxSize; j++)
			{
				if (Diamond[i][j] == 0)
				{
					printf("□");
				}
				else if (Diamond[i][j] == 1)
				{
					printf("■");
				}
			}
			printf("\n");

			if (i == MaxSize - 1)
			{
				printf("\n");
			}
		}

		for (int32_t i = first; i < end; i++)
		{
			Diamond[i][i + 1] = 1;
		}
		for (int32_t i = end; i > first; i--)
		{
			Diamond[i][i - 1] = 1;
		}

		first--;
		end++;

		Sleep(2000);
	}
}