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

void Maze02()
{
	Initialize_02();

	// --- マップ自動生成 ---
	// x:((0～11) + 2) * 2(偶数)
	// y:((0～11) + 2) * 2(偶数)
	FirstPosition.X = (rand() % (MAP_SIZE_X - 18) + 2) * 2;
	FirstPosition.Y = (rand() % (MAP_SIZE_Y - 18) + 2) * 2;
	COORD StartPosMin = { 0, 0 };	// 座標最小値
	COORD StartPosMax = { 28, 28 };	// 座標最大値

	// デバッグ用
	//FirstPosition.X = 24;
	//FirstPosition.Y = 12;

	// ゴール決定
	GoalFixed_02(MazeMap, FirstPosition);

	// 最初の道を作る 1.
	// 1.ループ回数で最初かそれ以外か判断
	// 四方向を見て作れるかどうか(TRUE/FALSE)を返す関数を作る
	while (itido)
	{
		if (LoopTimes == 0)
		{
			// ゴールから二マス分道を生成
			PathCreate_02(MazeMap, FirstPosition);
		}
		else
		{
			if (CheckIfCreatable(MazeMap, NextPosition) &&
				(NextPosition.X > StartPosMin.X && StartPosMax.X > NextPosition.X) &&
				(NextPosition.Y > StartPosMin.Y && StartPosMax.Y > NextPosition.Y))
			{
				// 次の座標から見て道を生成を繰り返す
				PathCreate_02(MazeMap, NextPosition);
			}
			else
			{
				StartFixed_02(MazeMap, NextPosition);
				itido = FALSE;
			}
		}
		LoopTimes++;
	}

	// 描画
	Draw();
}

void Initialize_02()
{
	// マップ
	for (short i = 0; i < MAP_SIZE_Y; i++)
	{
		for (short j = 0; j < MAP_SIZE_X; j++)
		{
			// 初期値は端に道を配置する
			//if (i == 0 || j == 0 || i == MAP_SIZE_Y - 1 || j == MAP_SIZE_X - 1)
			//{
			//	MazeMap[i][j] = EChip::Chip_Path;
			//}
			//else
			{
				MazeMap[i][j] = EChip::Chip_Wall;
			}
		}
	}

	ZeroMemory(SearchedMap, sizeof(SearchedMap));

	FirstPosition.X = FirstPosition.Y = 0;
	NextPosition.X = NextPosition.Y = 0;
	SavePosition.X = SavePosition.Y = 0;
	OpenDirection = 0;

	LoopTimes = 0;

	itido = TRUE;
}

void PathCreate_02(short Map[MAP_SIZE_Y][MAP_SIZE_X], COORD Position)
{
	// while文の条件用
	short SaveDirection = 0;
	bool ReCreateFlag = TRUE;
	short LoopCount = 0;		// 無限ループ対策
	COORD StartPosMin = { 0, 0 };	// 座標最小値
	COORD StartPosMax = { 28, 28 };	// 座標最大値
	// ループ用変数
	short j;	// 横
	short i;	// 縦

	// 2マス先の状態が壁なら、道にすることが出来る
	// 開く方向の決定
	OpenDirection = rand() % EDirection::Dir_Max;
	SaveDirection = OpenDirection;

	// Positionが配列外ではない/再生成条件を満たしている場合ループ続行
	while (((Position.X > StartPosMin.X && StartPosMax.X > Position.X) &&
		(Position.Y > StartPosMin.Y && StartPosMax.Y > Position.Y)))// &&
		//ReCreateFlag)
	{
		switch (OpenDirection)
		{
		case EDirection::Dir_Left:
			// 一番左にいるのであれば処理を飛ばす
			if (Position.X <= StartPosMin.X)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y][Position.X] = EChip::Chip_Path;
				break;
			}
			if (Map[Position.Y][Position.X - 2] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X - 2, Position.Y };
				// 道にする処理
				for (j = Position.X - 1; j >= NextPosition.X; j--)
				{
					Map[Position.Y][j] = EChip::Chip_Path;
					// 探索済みである
					//SearchedMap[Position.Y][j] = Map[Position.Y][j];
				}
			
				// 再生成の必要なし
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Up:
			// 一番上にいるのであれば処理を飛ばす
			if (Position.Y <= StartPosMin.Y)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y][Position.X] = EChip::Chip_Path;
				break;
			}

			if (Map[Position.Y - 2][Position.X] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X, Position.Y - 2 };
				// 道にする処理
				for (i = Position.Y - 1; i >= NextPosition.Y; i--)
				{
					Map[i][Position.X] = EChip::Chip_Path;
					// 探索済みである
					//SearchedMap[Position.Y][i] = Map[Position.Y][i];
				}

				// 再生成の必要なし
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Right:
			// 右の最大値にいるのであれば処理を飛ばす
			if (Position.X >= StartPosMax.X)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y][Position.X + 1] = EChip::Chip_Path;
				break;
			}

			if (Map[Position.Y][Position.X + 2] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X + 2, Position.Y };
				// 道にする処理
				for (j = Position.X + 1; j <= NextPosition.X; j++)
				{
					Map[Position.Y][j] = EChip::Chip_Path;
					// 探索済みである
					//SearchedMap[Position.Y][j] = Map[Position.Y][j];
				}

				// 再生成の必要なし
				ReCreateFlag = FALSE;
			}
			break;
		case EDirection::Dir_Down:
			// 前回の座標と同じであれば処理を飛ばす
			if (Position.Y >= StartPosMax.Y)
			{
				ReCreateFlag = TRUE;
				Map[Position.Y + 1][Position.X] = EChip::Chip_Path;
				break;
			}

			// 道にする処理
			if (Map[Position.Y + 2][Position.X] == EChip::Chip_Wall)
			{
				NextPosition = { Position.X, Position.Y + 2 };
				for (i = Position.Y + 1; i <= NextPosition.Y; i++)
				{
					Map[i][Position.X] = EChip::Chip_Path;
					// 探索済みである
					//SearchedMap[i][Position.X] = Map[i][Position.X];
				}

				// 再生成の必要なし
				ReCreateFlag = FALSE;
			}
			break;
		default:
			break;
		}

		// 再生成が必要となったら
		if (ReCreateFlag)
		{
			// 0->1, 1->2, 2->3, 3->0で一巡
			OpenDirection = ++OpenDirection % 4;
			LoopCount++;
			if (LoopCount == 100)
			{
				printf("再生成時無限ループしたので終わり\n");
				break;
			}
			// 元の座標(保存していた座標)に戻ってきたら(一巡した場合)
			if (SaveDirection == OpenDirection)
			{
				printf("一巡したので終わり\n");
				ReCreateFlag = FALSE;
			}

			ReCreateFlag = FALSE;
		}
		else
		{
			break;
		}

		if (LoopCount == 100)
		{
			printf("シンプルに無限ループしたので終わり\n");
			break;
		}
	}

	// 今回の座標を保存
	SavePosition = Position;
}

// スタートの決定
void StartFixed_02(short Map[][MAP_SIZE_X], COORD Position)
{
	Map[Position.Y][Position.X] = EChip::Chip_Start;
}
// ゴールの決定
void GoalFixed_02(short Map[][MAP_SIZE_X], COORD Position)
{
	Map[Position.Y][Position.X] = EChip::Chip_Goal;
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

	std::cout << FirstPosition.X << '\n' << FirstPosition.Y << '\n';
	std::cout << "ループ回数:" << LoopTimes << '\n';
}

// 四方向を見て道の生成が可能かどうかを見る
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