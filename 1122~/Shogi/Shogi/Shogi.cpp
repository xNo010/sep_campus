#include "Shogi.h"

int main()
{
	Initialize();
}

void Initialize()
{
	std::string moji[9] = 
	{
		"一", "二", "三", "四", "五", "六", "七", "八", "九"
	};

	for (int32_t i = 0; i < 9; i++)
	{
		Kanji[i] = moji[i];
	}

	ZeroMemory(PieceMap, sizeof(PieceMap));
	ZeroMemory(DrawMap, sizeof(DrawMap));

	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			if (i % 2 == 0)
			{
				if (j % 2 == 0)
				{
					DrawMap[i][j] = EChipCate::None;
				}
				else
				{
					DrawMap[i][j] = EChipCate::Space;
				}
			}
			else
			{
				if (j % 2 == 0)
				{
					DrawMap[i][j] = EChipCate::Space;
				}
				else
				{
					DrawMap[i][j] = EChipCate::Cross;
				}
			}
		}
	}

	Draw();
}

void Draw()
{
	// 列部分最初
	printf("　　9 　8 　7　 6 　5 　4 　3　 2 　1\n");
	for (int32_t j = 0; j < MAP_WIDTH; j++)
	{
		if (j == 0)
		{
			printf(" ┌　");
		}
		else if (j % 2 != 0 && j != MAP_WIDTH - 1)
		{
			printf(" ┬");
		}
		else if (j == MAP_WIDTH - 1)
		{
			printf(" ┐\n");
		}
		else
		{
			printf("　");
		}

	}

	for (int32_t i = 0, k = 0; i < MAP_HEIGHT && k < 9; i++)
	{
		if (i % 2 == 0)
		{
			printf("%s ", Kanji[k].c_str());
			k++;
		}
		else
		{
			printf(" ├");
		}

		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			switch (DrawMap[i][j])
			{
			case EChipCate::None:
				printf("□");
				break;
			case EChipCate::Space:
				printf("　");
				break;
			case EChipCate::Piece:
				printf("歩");
				//switch ()
				//{
				//default:
				//	break;
				//}
			case EChipCate::Cross:
				printf(" ┼");
				if (j == MAP_WIDTH - 1)
				{
					printf("\b┤");
				}
				break;
			default:
				break;
			}
		}
		printf("\n");
	}

	// 列部分最後
	for (int32_t j = 0; j < MAP_WIDTH; j++)
	{
		if (j == 0)
		{
			printf(" └　");
		}
		else if (j % 2 != 0 && j != MAP_WIDTH - 1)
		{
			printf(" ┴");
		}
		else if (j == MAP_WIDTH - 1)
		{
			printf(" ┘\n");
		}
		else
		{
			printf("　");
		}

	}
}