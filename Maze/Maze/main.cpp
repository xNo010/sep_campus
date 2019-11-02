#include "Maze01.h"
#include "Maze02.h"

int main()
{
	srand(timeGetTime());

	// 壁から作る迷路
	//Maze01();
	
	// 道から作る迷路
	Maze02();
}

void Maze01()
{
	// 初期化
	Initialize_01();

	// --- マップ自動生成 ---
	// x:2～13 * 2(偶数)
	// y:2～13 * 2(偶数)
	FirstPosition.X = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	FirstPosition.Y = (rand() % (MAP_SIZE_Y - 18) + 2) * 2;

	for (short j = 2; j < MAP_SIZE_Y - 3; j += 2)
	{
		for (short i = 2; i < MAP_SIZE_X - 3; i += 2)
		{
			FirstPosition.X = i;
			FirstPosition.Y = j;
			WallCreation(MazeMap, FirstPosition.X, FirstPosition.Y);
		}
	}

	// スタートとゴールの指定
	//StartFixed(MazeMap);
	//GoalFixed(MazeMap);

	// 描画
	Draw();
}

void Maze02()
{
	Initialize_02();

	// --- マップ自動生成 ---
	// x:((0～11) + 2) * 2(偶数)
	// y:((0～11) + 2) * 2(偶数)
	FirstPosition.X = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	FirstPosition.Y = (rand() % (MAP_SIZE_Y - 18) + 2) * 2;
	//if (FirstPosition.X == MAP_SIZE_X - 2)
	//{
	//	// マップを超えないよう1下げる
	//	FirstPosition.X = MAP_SIZE_X - 3;
	//}
	//if (FirstPosition.Y == MAP_SIZE_Y - 2)
	//{
	//	// マップを超えないよう1下げる
	//	FirstPosition.Y = MAP_SIZE_Y - 3;
	//}

	// ゴール決定
	GoalFixed_02(MazeMap, FirstPosition);

	// ゴールから道を生成(単発)
	PathCreate(MazeMap, FirstPosition);

	// 最初の道を作り上げるまで、NextPositionを見る:①
	// ①四方向を見て全て道であれば作れないこと(FALSE)を返す関数を作る
	// 作り終えた後の条件
	while (itido)
	{
		if (CheckIfCreatable(MazeMap, NextPosition))
		{
			PathCreate(MazeMap, NextPosition);
		}
		else
		{
			itido = FALSE;
			//// 乱数を再生成
			//// x:((0～12) + 2) * 2(偶数)
			//// y:((0～12) + 2) * 2(偶数)
			//NextPosition.X = (rand() % (MAP_SIZE_X - 17) + 2) * 2;
			//NextPosition.Y = (rand() % (MAP_SIZE_Y - 17) + 2) * 2;
			//if (NextPosition.X == MAP_SIZE_X - 2)
			//{
			//	// マップを超えないよう1下げる
			//	NextPosition.X = MAP_SIZE_X - 3;
			//}
			//if (NextPosition.Y == MAP_SIZE_Y - 2)
			//{
			//	// マップを超えないよう1下げる
			//	NextPosition.Y = MAP_SIZE_Y - 3;
			//}
			//
			//// そこが「ゴールでなく、すでに作られた道」となっている場所であれば
			//if (SearchedMap[NextPosition.Y][NextPosition.X] != EChip::Chip_Goal ||
			//	SearchedMap[NextPosition.Y][NextPosition.X] == EChip::Chip_Path)
			//{
			//	// そこから迷路の生成を再開
			//	PathCreate(MazeMap, NextPosition);
			//	itido = FALSE;
			//}
		}
	}

	// 描画
	Draw();
}

void Initialize_01()
{
	// マップ
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			// 壁をマップの端に初期で配置する
			if (i == 0 || j == 0 || i == MAP_SIZE_Y - 1 || j == MAP_SIZE_X - 1)
			{
				MazeMap[i][j] = EChip::Chip_Wall;
			}
			else
			{
				MazeMap[i][j] = EChip::Chip_Path;
			}
		}
	}

	FirstPosition.X = FirstPosition.Y = 0;
}

void Initialize_02()
{
	// マップ
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			// 初期値は端に道を配置する
			if (i == 0 || j == 0 || i == MAP_SIZE_Y - 1 || j == MAP_SIZE_X - 1)
			{
				MazeMap[i][j] = EChip::Chip_Path;
			}
			else
			{
				MazeMap[i][j] = EChip::Chip_Wall;
			}

			SearchedMap[i][j] = EChip::Chip_Wall;
		}
	}

	FirstPosition.X = FirstPosition.Y = 0;
	NextPosition.X = NextPosition.Y = 0;
	SavePosition.X = SavePosition.Y = 0;
	OpenDirection = 0;

	itido = TRUE;
}

void WallCreation(short Map[][MAP_SIZE_X], short x, short y)
{
	if (Map[y][x] != EChip::Chip_Path)
	{
		return;
	}
	// ランダムな場所に壁を配置
	Map[y][x] = EChip::Chip_Wall;

	// 生成方向の決定
	short Direction = rand() % 3;//EDirection::Dir_Max;

	// それぞれ2マス先を見て、道であれば隣に壁を生成する
	switch (Direction)
	{
	case EDirection::Dir_Left:
		if (Map[y][x - 2] == EChip::Chip_Path)
		{
			Map[y][x - 1] = EChip::Chip_Wall;
			WallCreation(Map, x - 2, y);
		}
		else if (Map[y][x - 2] == EChip::Chip_Wall)
		{
			Map[y][x - 1] = EChip::Chip_Wall;
		}
		break;
	case EDirection::Dir_Up:
		if (Map[y - 2][x] == EChip::Chip_Path)
		{
			Map[y - 1][x] = EChip::Chip_Wall;
			WallCreation(Map, x, y - 2);
		}
		else if (Map[y - 2][x] == EChip::Chip_Wall)
		{
			Map[y - 1][x] = EChip::Chip_Wall;
		}
		break;
	case EDirection::Dir_Right:
		if (Map[y][x + 2] == EChip::Chip_Path)
		{
			Map[y][x + 1] = EChip::Chip_Wall;
			WallCreation(Map, x + 2, y);
		}
		else if (Map[y][x + 2] == EChip::Chip_Wall)
		{
			Map[y][x + 1] = EChip::Chip_Wall;
		}
		break;
	case EDirection::Dir_Down:
		if (Map[y + 2][x] == EChip::Chip_Path)
		{
			Map[y + 1][x] = EChip::Chip_Wall;
			WallCreation(Map, x, y + 2);
		}
		else if (Map[y + 2][x] == EChip::Chip_Wall)
		{
			Map[y + 2][x] = EChip::Chip_Wall;
		}
		break;
	default:
		break;
	}
}

void PathCreate(short Map[][MAP_SIZE_X], COORD Position)
{
	// while文の条件用
	NextPosition = Position;
	short SaveDirection = 0;

	// ループ用変数
	short j;	// 横
	short i;	// 縦

	// 2マス先の状態が壁なら、道にすることが出来る
	// 開く方向の決定
	OpenDirection = rand() % EDirection::Dir_Max;
	SaveDirection = OpenDirection;

	// Nextが変わっていない(道を作れなかった)/4方向を見尽くしてOpenが一巡した場合ループ終了
	while ((Position.X == NextPosition.X && Position.Y == NextPosition.Y) ||
		SaveDirection == OpenDirection)
	{
		// 前回の座標と同じであれば処理を飛ばすif文を作る
		switch (OpenDirection)
		{
		case EDirection::Dir_Left:
			if (Map[Position.Y][Position.X - 2] == Map[SavePosition.Y][SavePosition.X])
				break;

			if (Map[Position.Y][Position.X - 2] == EChip::Chip_Wall)
			{
				// 道にする処理
				for (j = Position.X; j >= Position.X - 2; j--)
				{
					if (Map[Position.Y][j] != EChip::Chip_Goal)
					{
						Map[Position.Y][j] = EChip::Chip_Path;
						// 探索済みである
						SearchedMap[Position.Y][j] = Map[Position.Y][j];
					}
				}
				NextPosition.X = Position.X - 2;
			}
			break;
		case EDirection::Dir_Up:
			if (Map[Position.Y - 2][Position.X] == Map[SavePosition.Y][SavePosition.X])
				break;

			if (Map[Position.Y - 2][Position.X] == EChip::Chip_Wall)
			{
				// 道にする処理
				for (i = Position.Y; i >= Position.Y - 2; i--)
				{
					if (Map[i][Position.X] != EChip::Chip_Goal)
					{
						Map[i][Position.X] = EChip::Chip_Path;
						// 探索済みである
						SearchedMap[Position.Y][i] = Map[Position.Y][i];
					}
				}
				NextPosition.Y = Position.Y - 2;
			}
			break;
		case EDirection::Dir_Right:
			// 配列の範囲外に行かないように
			if (Position.X == 26)
			{
				if (Map[Position.Y][Position.X + 1] == Map[SavePosition.Y][SavePosition.X])
					break;

				// 道にする処理
				if (Map[Position.Y][Position.X + 1] == EChip::Chip_Wall)
				{
					for (j = Position.X; j <= Position.X + 1; j++)
					{
						if (Map[Position.Y][j] != EChip::Chip_Goal)
						{
							Map[Position.Y][j] = EChip::Chip_Path;
							// 探索済みである
							SearchedMap[Position.Y][j] = Map[Position.Y][j];
						}
					}
					NextPosition.X = Position.X + 1;
				}
			}
			else
			{
				if (Map[Position.Y][Position.X + 2] == Map[SavePosition.Y][SavePosition.X])
					break;

				if (Map[Position.Y][Position.X + 2] == EChip::Chip_Wall)
				{
					// 道にする処理
					for (j = Position.X; j <= Position.X + 2; j++)
					{
						if (Map[Position.Y][j] != EChip::Chip_Goal)
						{
							Map[Position.Y][j] = EChip::Chip_Path;
							// 探索済みである
							SearchedMap[Position.Y][j] = Map[Position.Y][j];
						}
					}
					NextPosition.X = Position.X + 2;
				}
			}
			break;
		case EDirection::Dir_Down:
			// 配列の範囲外に行かないように
			if (Position.Y == 26)
			{
				if (Map[Position.Y + 1][Position.X] == Map[SavePosition.Y][SavePosition.X])
					break;

				if (Map[Position.Y + 1][Position.X] == EChip::Chip_Wall)
				{
					// 道にする処理
					for (i = Position.Y; i <= Position.Y + 1; i++)
					{
						if (Map[i][Position.X] != EChip::Chip_Goal)
						{
							Map[i][Position.X] = EChip::Chip_Path;
							// 探索済みである
							SearchedMap[i][Position.X] = Map[i][Position.X];
						}
					}
					// 次回探索座標の更新
					NextPosition.Y = Position.Y + 1;
				}
			}
			else
			{
				if (Map[Position.Y + 2][Position.X] == Map[SavePosition.Y][SavePosition.X])
					break;

				// 道にする処理
				if (Map[Position.Y + 2][Position.X] == EChip::Chip_Wall)
				{
					for (i = Position.Y; i <= Position.Y + 2; i++)
					{
						if (Map[i][Position.X] != EChip::Chip_Goal)
						{
							Map[i][Position.X] = EChip::Chip_Path;
							// 探索済みである
							SearchedMap[i][Position.X] = Map[i][Position.X];
						}
					}
					// 次回探索座標の更新
					NextPosition.Y = Position.Y + 2;
				}
			}
			break;
		default:
			break;
		}
		// 0,1,2,3
		OpenDirection = ++OpenDirection % 4;
		// 乱数再生成
		//OpenDirection = rand() % EDirection::Dir_Max;
	}

	// 今回の座標を保存
	SavePosition = Position;
}

void StartFixed_01(short Map[][MAP_SIZE_X])
{
	short s_x = rand() % MAP_SIZE_X;
	short s_y = rand() % MAP_SIZE_Y;

	// 選ばれた番号が壁だった場合やり直し
	while (Map[s_y][s_x] == EChip::Chip_Wall)
	{
		s_x = rand() % MAP_SIZE_X;
		s_y = rand() % MAP_SIZE_Y;
	}

	Map[s_y][s_x] = EChip::Chip_Start;
}

void GoalFixed_01(short Map[][MAP_SIZE_X])
{
	short g_x = 0;
	short g_y = 0;
	bool AgeinFlag = FALSE;

	// 選ばれた番号が壁だった場合やり直し
	while (AgeinFlag || Map[g_y][g_x] == EChip::Chip_Wall)
	{
		g_x = rand() % MAP_SIZE_X;
		g_y = rand() % MAP_SIZE_Y;

		// 5マス→/←/↓/↑がマップ外だった場合やり直し
		if (g_x + 5 > MAP_SIZE_X || g_x - 5 < 0 ||
			g_y + 5 > MAP_SIZE_Y || g_y - 5 < 0)
		{
			AgeinFlag = TRUE;
			continue;
		}

		// ↓5マスの左右がスタートかどうかを見る


		/*else
		{
			// 縦横5マス以内にスタートがあった場合やり直し
			for (short j = g_y; j < g_y + 5; j++)
			{
				for (short i = g_x; i < g_x + 5; i++)
				{
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
				for (short i = g_x; i > g_x - 5; i--)
				{
					if (i > MAP_SIZE_X)
					{
						AgeinFlag = TRUE;
						break;
					}
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
			}
			for (short j = g_y; j > g_y - 5; j--)
			{
				for (short i = g_x; i < g_x + 5; i++)
				{
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
				for (short i = g_x; i > g_x - 5; i--)
				{
					if (i > MAP_SIZE_X)
					{
						AgeinFlag = TRUE;
						break;
					}
					if (Map[j][i] == EChip::Chip_Start)
					{
						AgeinFlag = TRUE;
					}
					else
					{
						AgeinFlag = FALSE;
					}
				}
			}
		}*/
	}

	Map[g_y][g_x] = EChip::Chip_Goal;
}

// ゴールの決定
void GoalFixed_02(short Map[][MAP_SIZE_X], COORD Position)
{
	if (Position.X < 0 || Position.X > MAP_SIZE_X - 1 ||
		Position.Y < 0 || Position.Y > MAP_SIZE_Y - 1)
	{
		printf("Map配列の範囲外です。\n");
		return;
	}

	Map[Position.Y][Position.X] = EChip::Chip_Goal;
	SearchedMap[Position.Y][Position.X] = Map[Position.Y][Position.X];
}

void Draw()
{
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			switch (MazeMap[i][j])
			{
			case EChip::Chip_Path:
				printf("□");
				break;
			case EChip::Chip_Wall:
				printf("■");
				break;
			case EChip::Chip_Start:
				printf("ス");
				break;
			case EChip::Chip_Goal:
				printf("ゴ");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}

	std::cout << NextPosition.X << '\n' << NextPosition.Y << '\n';

	//std::cout << "Hello World!\n";
	//std::cout << FirstPosition.x << '\n' << FirstPosition.y << '\n';
	//int x = 0;
	//for (int i = 0; i < 10; i++)
	//{
	//	x = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	//	std::cout << "乱数テスト:" << x << '\n';
	//}

}

// 四方向を見て生成可能かどうかを見る
bool CheckIfCreatable(short Map[][MAP_SIZE_X], COORD CheckPos)
{
	// 4方向それぞれ2マス先を見て、道になっていたらCheckPosからは作れない
	// 左/上/右/下方向
	if (Map[CheckPos.Y][CheckPos.X - 2] == EChip::Chip_Path &&
		Map[CheckPos.Y - 2][CheckPos.X] == EChip::Chip_Path &&
		Map[CheckPos.Y][CheckPos.X + 2] == EChip::Chip_Path &&
		Map[CheckPos.Y + 2][CheckPos.X] == EChip::Chip_Path)
	{
		return FALSE;
	}
	return TRUE;
}