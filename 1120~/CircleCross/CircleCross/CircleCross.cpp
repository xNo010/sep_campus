#include "CircleCross.h"

int main()
{
	Initialize();

	do
	{
		while (!FinishCheck(Map))
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
	NowTurn = true;
	HoriCheck = 0;
	VertCheck = 0;
	SaveTotal = 0;

	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			if (i % 2 != 0)
			{
				if (j % 2 != 0)
				{
					Map[i][j] = EChipCate::VertLine;
				}
				else
				{
					Map[i][j] = EChipCate::HoriLine;
				}
			}
			else
			{
				if (j % 2 != 0)
				{
					Map[i][j] = EChipCate::VertLine;
				}
				else
				{
					Map[i][j] = EChipCate::Square;
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
		CharChange = 2;
		break;
	case 'C':
		CharChange = 4;
		break;
	default:
		break;
	}

	// 縦番号によって、配列番号用に補正値を変更
	int32_t offset = 0;
	switch (Input.VertSelect)
	{
	case 1:
		offset = -1;
		break;
	case 2:
		offset = 0;
		break;
	case 3:
		offset = 1;
		break;
	default:
		break;
	}

	HoriCheck = CharChange;
	VertCheck = Input.VertSelect + offset;

	if (SelectedCheck(Map, HoriCheck, VertCheck))
	{
		printf("そこは選択済み\n");
		return;
	}

	// マップに配置(NowTurnで配置記号が変わる)
	if (NowTurn)
	{
		Map[Input.VertSelect + offset][CharChange] = EChipCate::Circle;
	}
	else
	{
		Map[Input.VertSelect + offset][CharChange] = EChipCate::Cross;
	}

	// 交代
	NowTurn = !NowTurn;

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
			switch (Map[i][j])
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

bool SelectedCheck(int32_t Map[MAP_HEIGHT][MAP_WIDTH], int32_t Hori, int32_t Vert)
{
	if (Map[Vert][Hori] != EChipCate::Square)
	{
		return true;
	}

	return false;
}

bool FinishCheck(int32_t Map[MAP_HEIGHT][MAP_WIDTH])
{
	const int32_t MaxCount = 9;

	// 最初に全部四角なのか見る(判定の必要があるかどうか)
	int32_t SquareCount = 0;
	for (int32_t i = 0; i < MAP_HEIGHT; i += 2)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j += 2)
		{
			if (Map[i][j] == EChipCate::Square)
			{
				SquareCount++;
			}
		}
	}
	// 四角が5個以上あったら揃わない(false)
	if (SquareCount >= MaxCount - 4)
	{
		return false;
	}

	
	const int32_t FinishCount = 3;
	int32_t TotalSelected = 0;
	
	// 自分を見るのは、自分じゃないときのターン
	// 縦列を軸に、横列、斜め列も見ていく
	int32_t HoriCount[MAP_WIDTH / 2] = { 0, };
	int32_t ObliqueCount = 0;
	int32_t InvObliqueCount = 0;
	for (int32_t j = 0; j < MAP_WIDTH; j += 2)
	{
		int32_t VertCount = 0;
		if (!NowTurn)
		{
			for (int32_t i = 0; i < MAP_HEIGHT; i += 2)
			{
				if (Map[i][j] == EChipCate::Square)
					continue;

				if (Map[i][j] == EChipCate::Circle)
				{
					VertCount++;
					HoriCount[i / 2]++;
				}
				else
				{
					TotalSelected++;
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
			for (int32_t i = 0; i < MAP_HEIGHT; i += 2)
			{
				if (Map[i][j] == EChipCate::Square)
					continue;

				if (Map[i][j] == EChipCate::Cross)
				{
					VertCount++;
					HoriCount[i / 2]++;
				}
				else
				{
					TotalSelected++;
				}
			}

			if (VertCount == FinishCount)
			{
				printf("×の勝ち\n");
				return true;
			}
		}
		TotalSelected += VertCount;
	}

	// 横列の判定
	for (int32_t j = 0; j < MAP_WIDTH / 2; j++)
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
	
	// 正斜め列()
	for (int32_t i = 0; i < MAP_HEIGHT; i += 2)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j += 2)
		{
			if (i != j)
				continue;

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
	}

	// 逆斜め列()
	for (int32_t i = 0; i < MAP_HEIGHT; i += 2)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j += 2)
		{
			if ((i != 0 || j != MAP_WIDTH - 2))
				continue;
				
			if (!NowTurn)
			{
				if (Map[i][j] == EChipCate::Circle && 
					Map[j][i] == EChipCate::Circle &&
					Map[i + 2][j - 2] == EChipCate::Circle)
				{
					printf("◯の勝ち\n");
					return true;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (Map[i][j] == EChipCate::Cross &&
					Map[j][i] == EChipCate::Cross &&
					Map[i + 2][j - 2] == EChipCate::Cross)
				{
					printf("×の勝ち\n");
					return true;
				}
				else
				{
					break;
				}
			}
		}
	}

	if (TotalSelected == MaxCount)
	{
		printf("引き分けです\n");
		return true;
	}

	return false;
}