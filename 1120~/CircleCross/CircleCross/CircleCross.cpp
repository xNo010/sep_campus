#include "CircleCross.h"

int main()
{
	Initialize();

	do
	{
		while (!FinishCheck(CheckMap, WhatHandCount))
		{
			InputFunc();
	
			Update();
		}
	} while (Again());
}

void Initialize()
{
	Input.HoriSelect = '\0';
	Input.VertSelect = 0;
	HoriPos = 0;
	VertPos = 0;
	WhatHandCount = 0;
	TotalSelected = 0;
	NowTurn = true;

	for (int32_t i = 0; i < HALF_MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < HALF_MAP_WIDTH; j++)
		{
			CheckMap[i][j] = EChipCate::Square;
		}
	}

	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			if (i % 2 != 0)
			{
				if (j % 2 != 0)
				{
					DrawMap[i][j] = EChipCate::VertLine;
				}
				else
				{
					DrawMap[i][j] = EChipCate::HoriLine;
				}
			}
			else
			{
				if (j % 2 != 0)
				{
					DrawMap[i][j] = EChipCate::VertLine;
				}
				else
				{
					DrawMap[i][j] = EChipCate::Square;
				}
			}
		}
	}

	Draw();
}

void InputFunc()
{
	if (NowTurn)
	{
		printf("\n◯の人\n");
	}
	else
	{
		printf("\n×の人\n");
	}

	printf("横番号の入力(A,B,C):");
	std::cin >> Input.HoriSelect;

	printf("縦番号の入力(1,2,3):");
	scanf_s("%d", &Input.VertSelect);
}

void Update()
{
	if (Input.VertSelect < 1 || 3 < Input.VertSelect ||
		Input.HoriSelect < 'A' || 'C' < Input.HoriSelect)
	{
		printf("だめです\n");
		return;
	}

	// 文字を数値に変換
	int32_t CharChange = 0;
	switch (Input.HoriSelect)
	{
	case 'A':
		CharChange = 0;
		break;
	case 'B':
		CharChange = 1;
		break;
	case 'C':
		CharChange = 2;
		break;
	default:
		break;
	}

	// 縦番号の補正値
	int32_t offset = -1;
	HoriPos = CharChange;
	VertPos = Input.VertSelect + offset;

	if (SelectedCheck(CheckMap, HoriPos, VertPos))
	{
		printf("そこは選択済み\n");
		return;
	}

	// マップに配置(NowTurnで配置記号が変わる)
	if (NowTurn)
	{
		CheckMap[VertPos][HoriPos] = EChipCate::Circle;
	}
	else
	{
		CheckMap[VertPos][HoriPos] = EChipCate::Cross;
	}

	// 何手目かを知る
	WhatHandCount++;

	// 交代
	NowTurn = !NowTurn;

	// 判定用マップを描画マップに送る
	for (int32_t i = 0; i < HALF_MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < HALF_MAP_WIDTH; j++)
		{
			if (CheckMap[i][j] == EChipCate::Square)
				continue;

			DrawMap[i * 2][j * 2] = CheckMap[i][j];
		}
	}


	Draw();
}

bool Again()
{
	char StartorEnd[10] = "";
	printf("もう一度やりますか(はい/いいえ)\n");
	scanf_s("%10s", &StartorEnd, 10);

	int32_t len = 0;
	while (StartorEnd[len] != 0)
	{
		len++;
	}

	if (len == 4)
	{
		Initialize();
		return true;
	}
	else
	{
		printf("終了\n");
	}

	return false;
}

void Draw()
{
	int32_t offset = 1;

	printf("\n　A　B　C\n");
	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		// 左端に入る縦番号
		if (i % 2 == 0)
		{
			printf("%d", i + offset);
			offset--;
		}
		else
		{
			printf(" ");
		}

		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			switch (DrawMap[i][j])
			{
			case EChipCate::None:
				printf(" ");
				break;
			case EChipCate::Square:
				printf("□");
				break;
			case EChipCate::Circle:
				printf("◯");
				break;
			case EChipCate::Cross:
				printf("×");
				break;
			case EChipCate::VertLine:
				printf("|");
				break;
			case EChipCate::HoriLine:
				printf("―");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
}

bool SelectedCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], int32_t Hori, int32_t Vert)
{
	// 四角でなければ選択されてる
	if (Map[Vert][Hori] != EChipCate::Square)
	{
		return true;
	}

	return false;
}

bool FinishCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], int32_t WhatHandCount)
{
	const int32_t MaxCount = 9;

	// 5手目以降判定をする/それ以外はすぐfalseを返す
	if (WhatHandCount < MaxCount - 4)
	{
		return false;
	}

	const int32_t FinishCount = 3;
	
	if (MatrixCheck(Map, FinishCount))
	{
		return true;
	}
	
	if (ObliqueCheck(Map, FinishCount))
	{
		return true;
	}

	if (TotalSelected == MaxCount)
	{
		printf("引き分けです\n");
		return true;
	}

	return false;
}

bool MatrixCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], const int32_t FinishCount)
{
	// 自分を見るのは、自分じゃないときのターン

	// どれだけ選択されてるか
	int32_t SelectedNum = 0;

	// 縦列を軸に、横列も見る
	int32_t HoriCount[HALF_MAP_WIDTH] = { 0, };
	for (int32_t j = 0; j < HALF_MAP_WIDTH; j++)
	{
		int32_t VertCount = 0;
		if (!NowTurn)
		{
			for (int32_t i = 0; i < HALF_MAP_HEIGHT; i++)
			{
				if (Map[i][j] == EChipCate::Square)
					continue;

				if (Map[i][j] == EChipCate::Circle)
				{
					VertCount++;
					HoriCount[i]++;
				}
				else
				{
					SelectedNum++;
				}
			}

			if (VertCount == FinishCount)
			{
				printf("◯の勝ち\n");
				return true;
			}
		}
		else
		{
			for (int32_t i = 0; i < HALF_MAP_HEIGHT; i++)
			{
				if (Map[i][j] == EChipCate::Square)
					continue;

				if (Map[i][j] == EChipCate::Cross)
				{
					VertCount++;
					HoriCount[i]++;
				}
				else
				{
					SelectedNum++;
				}
			}

			if (VertCount == FinishCount)
			{
				printf("×の勝ち\n");
				return true;
			}
		}
		SelectedNum += VertCount;
	}

	// 横列の判定
	for (int32_t j = 0; j < HALF_MAP_WIDTH; j++)
	{
		if (HoriCount[j] == FinishCount)
		{
			if (!NowTurn)
			{
				printf("◯の勝ち\n");
				return true;
			}
			else
			{
				printf("×の勝ち\n");
				return true;
			}
		}
	}

	// 合計を保存
	TotalSelected = SelectedNum;

	return false;
}

bool ObliqueCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], const int32_t FinishCount)
{
	int32_t ObliqueCount = 0;
	int32_t InvObliqueCount = 0;

	// 正斜め列
	for (int32_t j = 0, i = 0; j < HALF_MAP_WIDTH && i < HALF_MAP_HEIGHT; j++, i++)
	{
		if (!NowTurn)
		{
			if (Map[i][j] == EChipCate::Circle)
			{
				ObliqueCount++;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (Map[i][j] == EChipCate::Cross)
			{
				ObliqueCount++;
			}
			else
			{
				break;
			}
		}
	}

	if (ObliqueCount == FinishCount)
	{
		if (!NowTurn)
		{
			printf("◯の勝ち\n");
			return true;
		}
		else
		{
			printf("×の勝ち\n");
			return true;
		}
	}

	// 逆斜め列()
	for (int32_t i = 0, j = HALF_MAP_WIDTH - 1; i < HALF_MAP_HEIGHT && j >= 0; i++, j--)
	{
		if (!NowTurn)
		{
			if (Map[i][j] == EChipCate::Circle)
			{
				InvObliqueCount++;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (Map[i][j] == EChipCate::Cross)
			{
				InvObliqueCount++;
			}
			else
			{
				break;
			}
		}
	}

	if (InvObliqueCount == FinishCount)
	{
		if (!NowTurn)
		{
			printf("◯の勝ち\n");
			return true;
		}
		else
		{
			printf("×の勝ち\n");
			return true;
		}
	}

	return false;
}