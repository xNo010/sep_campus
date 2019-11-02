// tetris.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

#include "tetris.h"

int main()
{
	if (!Initialize())
	{
		printf("初期化に失敗\n");
		return 0;
	}

	while (!EndFlag)
	{
		// 入力情報
		if (!PushFlag && GetKeyState(VK_LEFT) == 1)
		{
			// 最初の移動
			if (NewPosition == Position)
			{
				NewPosition = Position - 1;
				ZeroMemory(MoveBlock, sizeof(MoveBlock));

				for (short i = 0; i < BLOCK_NUM; i++)
				{
					for (short j = 0; j < BLOCK_NUM; j++)
					{
						if (j != 1)
							continue;
				
						Map[i][Position] = EBlock::None;
						MoveBlock[i][j] = EBlock::Exist;
						Map[i][NewPosition] = MoveBlock[i][j];
					}
				}
			}
			else
			{
				for (short i = 0; i < BLOCK_NUM; i++)
				{
					for (short j = 0; j < BLOCK_NUM; j++)
					{
						if (j != 1)
							continue;
				
						MoveBlock[i][j] = EBlock::Exist;
						Map[i][NewPosition] = MoveBlock[i][j];
					}
				}
				NewPosition--;
			}

			printf("←キー押されました\n");
			PushFlag = TRUE;
		}
		else if (!PushFlag && GetKeyState(VK_RIGHT) == 1)
		{
			// 最初の移動
			if (NewPosition == Position)
			{
				NewPosition = Position + 1;
				ZeroMemory(MoveBlock, sizeof(MoveBlock));

				for (short i = 0; i < BLOCK_NUM; i++)
				{
					for (short j = 0; j < BLOCK_NUM; j++)
					{
						if (j != 1)
							continue;
				
						Map[i][Position] = EBlock::None;
						MoveBlock[i][j] = EBlock::Exist;
						Map[i][NewPosition] = MoveBlock[i][j];
					}
				}
			}
			else
			{
				//if (NewPosition == )
				//for (short i = 0; i < BLOCK_NUM; i++)
				//{
				//	for (short j = 0; j < BLOCK_NUM; j++)
				//	{
				//		if (j != 1)
				//			continue;
				//
				//		MoveBlock[i][j] = EBlock::Exist;
				//		Map[i][NewPosition] = MoveBlock[i][j];
				//	}
				//}
				//NewPosition--;
			}

			printf("→キー押されました\n");
			PushFlag = TRUE;
		}
		else
		{
			PushFlag = FALSE;
		}

		if (DrawFlag)
		{
			Draw();
			DrawFlag = FALSE;
		}

		if (PushFlag)
		{
			//UpdateBlock(NewPosition);
			DrawFlag = TRUE;
			Sleep(1000);
		}
	}
	
	return 0;
}

bool Initialize()
{
	srand(timeGetTime());

	ZeroMemory(Map, sizeof(Map));
	ZeroMemory(Zangai, sizeof(Zangai));
	ZeroMemory(MoveBlock, sizeof(MoveBlock));

	// すでに置かれたブロックの配置
	for (short i = MAP_HEIGHT - 2; i < MAP_HEIGHT; i++)
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			if (i == MAP_HEIGHT - 2)
			{
				Zangai[i][j] = rand() % 4;
				if (Zangai[i][j] >= 2)
					Zangai[i][j] = 0;
			}
			if (Zangai[i - 1][j] == EBlock::Exist)
			{
				Zangai[i][j] = EBlock::Exist;
			}

			Map[i][j] = Zangai[i][j];
		}
	}

	// 縦棒作る
	Position = 4;
	for (short i = 0; i < BLOCK_NUM; i++)
	{
		for (short j = 0; j < BLOCK_NUM; j++)
		{
			if (j != 1)
				continue;

			MoveBlock[i][j] = EBlock::Exist;
			Map[i][Position] = MoveBlock[i][j];
		}
	}

	NewPosition = Position;

	UpdateCount = 0;

	PushFlag = FALSE;
	DrawFlag = TRUE;
	EndFlag = FALSE;

	// キー入力情報の取得
	if (!GetKeyboardState(Key))
	{
		printf("キー情報の取得に失敗\n");
	}

	return TRUE;
}

void UpdateBlock(long NewPosition)
{
	for (short i = 0; i < BLOCK_NUM + UpdateCount; i++)
	{
		if (i >= MAP_HEIGHT)
		{
			break;
		}
		for (short j = 0; j < BLOCK_NUM; j++)
		{
			if (j != 1)
				continue;

			Map[UpdateCount][NewPosition] = EBlock::None;
			Map[i + UpdateCount][NewPosition] = MoveBlock[i][j];
		}
	}
	UpdateCount++;
}

void Draw()
{
	// マップの描画
	for (short i = 0; i < MAP_HEIGHT; i++)
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			switch (Map[i][j])
			{
			case EBlock::None:
				// 図形の表示
				printf("□");
				break;
			case EBlock::Exist:
				// 図形の表示
				printf("■");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
}