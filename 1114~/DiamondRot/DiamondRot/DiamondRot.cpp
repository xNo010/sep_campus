#include <iostream>
#include <Windows.h>

#define MaxSize 7
#define DiaCenter ((MaxSize - 1) / 2)

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

	Diamond[DiaCenter][DiaCenter] = 1;

	first = DiaCenter;
	end = DiaCenter;

	while (first >= 0 && end < MaxSize)
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

		// 中心から上下左右に
		Diamond[first - 1][first] = Diamond[first][first];
		Diamond[first + 1][end] = Diamond[first][first];
		Diamond[end][first - 1] = Diamond[first][first];
		Diamond[end][end + 1] = Diamond[first][first];

		// 元の場所は消す
		Diamond[first][first] = 0;
		Diamond[first][end] = 0;
		Diamond[end][first] = 0;
		Diamond[end][end] = 0;
	
		first--;
		end++;

		Sleep(2000);
	}
}