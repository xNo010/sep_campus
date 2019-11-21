#include "Hanoi.h"

int main()
{
	printf("ハノイの塔\n");
	Initialize();

	Execute();
}

// 初期化
void Initialize()
{
	// 移動元タワー番号
	BeforeNumber = 0;
	// 動きたいタワー番号
	MoveNumber = 0;

	// マップの初期化
	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			MapData[i][j] = false;
		}
	}

	// 塔を作る
	// 塔の補正ポジション
	int32_t offset = 2;
	for (int32_t k = 0; k < MAX_TOWER; k++)
	{
		Tower[k].Pos = { offset + (13 * k), offset };
		Tower[k].Flag = 0;

		for (int32_t i = 0; i < TOWER_HEIGHT; i++)
		{
			for (int32_t j = 0; j < TOWER_WIDTH; j++)
			{
				if (k == 0)
				{
					if (j == TOWER_CENTER || i == TOWER_HEIGHT - 1)
					{
						Tower[0].Data[i][j] = true;
						Tower[0].Data[TOWER_HEIGHT - 1][j] = true;
					}
					else
					{
						Tower[0].Data[i][j] = false;
					}
				}
				else
				{
					Tower[k].Data[i][j] = Tower[0].Data[i][j];
				}
			}
		}
	}

	// オブジェクトの生成/塔に配置
	for (int32_t i = 0; i < MAX_OBJECT; i++)
	{
		// オブジェクト初期ポジション/左端の塔に刺すように
		Object[i].InitPos = { TOWER_CENTER - Tower[0].Pos.x + (i - 1), TOWER_HEIGHT - Tower[0].Pos.y - (offset * i) };
		Object[i].Pos = Object[i].InitPos;

		// 大きいやつから作る
		Object[i].Size = MAX_OBJSIZE - (SIZEDIFF * i);

		for (int32_t j = 0; j < MAX_OBJSIZE; j++)
		{
			if (j < Object[i].Size)
			{
				Object[i].Data[j] = true;
			}
			else
			{
				Object[i].Data[j] = false;
			}

			if (Object[i].Data[j] == false)
				continue;

			Tower[0].Data[Object[i].InitPos.y][Object[i].InitPos.x + j] = Object[i].Data[j];
		}
		Tower[0].Flag++;
	}

	// 塔をマップに配置
	for (int32_t k = 0; k < MAX_TOWER; k++)
	{
		for (int32_t i = 0; i < TOWER_HEIGHT; i++)
		{
			for (int32_t j = 0; j < TOWER_WIDTH; j++)
			{
				if (Tower[k].Data[i][j] == false)
					continue;

				MapData[Tower[k].Pos.y + i][Tower[k].Pos.x + j] = Tower[k].Data[i][j];
			}
		}
	}

	// 初期状態を表示
	Draw();
}

void Execute()
{
	// クリア条件達成までループ
	while (Tower[MAX_TOWER - 1].Flag != 3)
	{
		printf("\n移動したいオブジェクトがあるタワー番号を入力(0/1/2):");
		scanf_s("%d", &BeforeNumber);
		if ((BeforeNumber < 0 || BeforeNumber > 2) || Tower[BeforeNumber].Flag == 0)
		{
			printf("そのタワーにはオブジェクトがありません\n");
			continue;
		}
	
		printf("\n移動したいタワー番号を入力(0/1/2):");
		scanf_s("%d", &MoveNumber);
		if ((MoveNumber < 0 || MoveNumber > 2))
		{
			printf("そのタワー番号は存在しません\n");
			continue;
		}
		Update(BeforeNumber, MoveNumber);
	}

	printf("Game Clear!\n");
}

void Update(int32_t BeforeNumber, int32_t MoveNumber)
{
	//																	 置きたいオブジェクトのサイズ				置きたいタワーにあるオブジェクトのサイズ
	//if (Tower[MoveNumber].Flag == 3 || (Tower[MoveNumber].Flag != 0 && Object[Tower[BeforeNumber].Flag - 1].Size > Object[Tower[MoveNumber].Flag - 1].Size))
	//{
	//	printf("そこには置けません\n");
	//	return;
	//}

	// 選択オブジェクトの縦移動
	switch (Object[Tower[BeforeNumber].Flag - 1].Size)
	{
	case ESizeCate::Min:
		break;
	case ESizeCate::Middle:
		break;
	case ESizeCate::Max:
		break;
	default:
		break;
	}
	Object[Tower[BeforeNumber].Flag - 1].Pos.y = Object[(MAX_OBJECT - 1) - Tower[MoveNumber].Flag].InitPos.y;

	// マップの初期化
	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			MapData[i][j] = false;
		}
	}

	// 塔を作る
	for (int32_t k = 0; k < MAX_TOWER; k++)
	{
		Tower[k].Flag = 0;

		for (int32_t i = 0; i < TOWER_HEIGHT; i++)
		{
			for (int32_t j = 0; j < TOWER_WIDTH; j++)
			{
				if (k == 0)
				{
					if (j == TOWER_CENTER || i == TOWER_HEIGHT - 1)
					{
						Tower[0].Data[i][j] = true;
						Tower[0].Data[TOWER_HEIGHT - 1][j] = true;
					}
					else
					{
						Tower[0].Data[i][j] = false;
					}
				}
				else
				{
					Tower[k].Data[i][j] = Tower[0].Data[i][j];
				}
			}
		}
	}

	// オブジェクトを塔に配置
	for (int32_t i = 0; i < MAX_OBJECT; i++)
	{
		//if (i == ((MAX_OBJECT - 1) - Tower[MoveNumber].Flag))
		//	continue;

		for (int32_t j = 0; j < MAX_OBJSIZE; j++)
		{
			if (Object[i].Data[j] == false)
				continue;

			Tower[0].Data[Object[i].Pos.y][Object[i].Pos.x + j] = Object[i].Data[j];
		}
		Tower[0].Flag++;
	}

	//// 移動したオブジェクトを指定の塔に配置
	//for (int32_t j = 0; j < MAX_OBJSIZE; j++)
	//{
	//	if (Object[(MAX_OBJECT - 1) - Tower[MoveNumber].Flag].Data[j] == false)
	//		continue;
	//
	//	Tower[MoveNumber].Data[Object[(MAX_OBJECT - 1) - Tower[MoveNumber].Flag].Pos.y][Object[(MAX_OBJECT - 1) - Tower[MoveNumber].Flag].Pos.x + j]
	//		= Object[(MAX_OBJECT - 1) - Tower[MoveNumber].Flag].Data[j];
	//}
	//Tower[MoveNumber].Flag++;

	// 塔をマップに配置
	for (int32_t k = 0; k < MAX_TOWER; k++)
	{
		for (int32_t i = 0; i < TOWER_HEIGHT; i++)
		{
			for (int32_t j = 0; j < TOWER_WIDTH; j++)
			{
				if (Tower[k].Data[i][j] == false)
					continue;

				MapData[Tower[k].Pos.y + i][Tower[k].Pos.x + j] = Tower[k].Data[i][j];
			}
		}
	}

	Draw();
}

// 描画
void Draw()
{
	for (int32_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (int32_t j = 0; j < MAP_WIDTH; j++)
		{
			switch (MapData[i][j])
			{
			case false:
				printf("■");
				break;
			case true:
				printf("□");
				break;
			}
		}
		printf("\n");
	}
	printf("　　　　　　　0　　　　　　　　　　　　　1　　　　　　　　　　　　2\n");
	printf("%d, %d, %d\n", Tower[0].Flag, Tower[1].Flag, Tower[2].Flag);
}