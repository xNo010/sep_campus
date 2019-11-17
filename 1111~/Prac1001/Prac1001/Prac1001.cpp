#include <iostream>

struct POINT
{
	int x;
	int y;
};

int main()
{
	struct POINT aBigDipper[] =
	{
		{8, 5}, {11, 1}, {25, 2},
		{26, 6}, {36, 9}, {46, 12}, {58, 12}
	};

	char space[15][61];
	int i, j;
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 60; j++)
		{
			space[i][j] = ' ';
		}
		space[i][j] = 0;
	}

	for (i = 0; i < 7; i++)
	{
		space[aBigDipper[i].y][aBigDipper[i].x] = '*';
	}
	for (i = 0; i < 15; i++)
	{
		printf("%s\n", space[i]);
	}
}