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
	ZeroMemory(Shogi, sizeof(Shogi));
	ZeroMemory(ShogiHistory, sizeof(ShogiHistory));

	// 駒用マップと情報格納
	for (int32_t i = 0, k = 0; i < VERT_NUM && k < PIECE_ALLOCNUM; i++)
	{
		switch (i)
		{
		case 0:
		case VERT_NUM - 1:
			for (int32_t j = 0; j < HORI_NUM; j++)
			{
				switch (j)
				{
				case 0:
				case HORI_NUM - 1:
					PieceMap[i][j] = EChipCate::Piece_Lance;
					break;
				case 1:
				case HORI_NUM - 2:
					PieceMap[i][j] = EChipCate::Piece_Knight;
					break;
				case 2:
				case HORI_NUM - 3:
					PieceMap[i][j] = EChipCate::Piece_SilverGeneral;
					break;
				case 3:
				case HORI_NUM - 4:
					PieceMap[i][j] = EChipCate::Piece_GoldGeneral;
					break;
				default:
					PieceMap[i][j] = EChipCate::Piece_King;
					break;
				}

				if (i == 0)
				{
					Shogi[k].Which = false;
				}
				else
				{
					Shogi[k].Which = true;
				}
				Shogi[k].Pos = { j, i };
				Shogi[k].Piece = PieceMap[i][j];
				k++;
			}
			break;
		case 1:
		case VERT_NUM - 2:
			for (int32_t j = 0; j < HORI_NUM; j++)
			{
				switch (j)
				{
				case 1:
					PieceMap[i][j] = EChipCate::Piece_Bishop;
					break;
				case HORI_NUM - 2:
					PieceMap[i][j] = EChipCate::Piece_Rook;
					break;
				default:
					PieceMap[i][j] = EChipCate::Piece_None;
					break;
				}

				if (PieceMap[i][j] == EChipCate::Piece_None)
					continue;

				if (i == 1)
				{
					Shogi[k].Which = false;
				}
				else
				{
					Shogi[k].Which = true;
				}
				Shogi[k].Pos = { j, i };
				Shogi[k].Piece = PieceMap[i][j];
				k++;
			}
			break;
		case 2:
		case VERT_NUM - 3:
			for (int32_t j = 0; j < HORI_NUM; j++)
			{
				PieceMap[i][j] = EChipCate::Piece_Pawn;

				if (i == 2)
				{
					Shogi[k].Which = false;
				}
				else
				{
					Shogi[k].Which = true;
				}
				Shogi[k].Pos = { j, i };
				Shogi[k].Piece = PieceMap[i][j];
				k++;
			}
			break;
		default:
			for (int32_t j = 0; j < HORI_NUM; j++)
			{
				PieceMap[i][j] = EChipCate::Piece_None;
			}
			break;
		}
	}

	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			if (i % 2 == 0)
			{
				if (j % 2 == 0)
				{
					DrawMap[i][j] = PieceMap[i / 2][j / 2];
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

	printf("将棋を始めます\n");
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
			case EChipCate::Piece_None:
				printf("□");
				break;
			case EChipCate::Piece_King:
				printf("王");
				break;
			case EChipCate::Piece_GoldGeneral:
				printf("金");
				break;
			case EChipCate::Piece_SilverGeneral:
				printf("銀");
				break;
			case EChipCate::Piece_Knight:
				printf("桂");
				break;
			case EChipCate::Piece_Lance:
				printf("香");
				break;
			case EChipCate::Piece_Rook:
				printf("飛");
				break;
			case EChipCate::Piece_Bishop:
				printf("角");
				break;
			case EChipCate::Piece_Pawn:
				printf("歩");
				break;
			case EChipCate::Space:
				printf("　");
				break;
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