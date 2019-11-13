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
		// --- 入力情報 ---
		// 左・右回り回転:ADキー
		if (GetKeyState('A') & 0x80)
		{
			printf("左回りに回転\n");
			RotateDir--;
			if (RotateDir < ERotationType::Standard)
			{
				RotateDir = ERotationType::Left;
			}
			printf("回転値:%d\n", RotateDir);
			LeftRotateFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, &EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}
		if (GetKeyState('D') & 0x80)
		{
			printf("右回りに回転\n");
			RotateDir++;
			RotateDir %= 4;
			printf("回転値:%d\n", RotateDir);
			LeftRotateFlag = FALSE;

			// 更新
			Update(&NewPosition, RotateDir, &EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}

		// 左右移動:←→キー
		if (!LeftMoveFlag && GetKeyState(VK_LEFT) & 0x80)
		{
			printf("左に1マス移動\n");
			NewPosition.X -= MovingValue;
			LeftMoveFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, &EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}
		else if (!RightMoveFlag && GetKeyState(VK_RIGHT) & 0x80)
		{
			printf("右に1マス移動\n");
			NewPosition.X += MovingValue;
			RightMoveFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, &EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}

		// 高速落下
		if (!PushDownFlag && GetKeyState(VK_SPACE) & 0x80)
		{
			printf("自分で落下\n");
			PushDownFlag = TRUE;
		}

		// ブロックの落下
		//if (!PushReturnFlag && GetKeyState(VK_RETURN) & 0x80)
		{
			NewPosition.Y += MovingValue;
			PushReturnFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, &EndFlag);

			if (PushDownFlag)
			{
				// ループを早くする
				Sleep(200);
				PushDownFlag = FALSE;
			}
			else
			{
				// 通常の待機時間
				//Sleep(100);
				Sleep(1000);
			}
		}
	}

	printf("テトリス終了\n");
}

bool Initialize()
{
	srand(timeGetTime());

	// キー入力情報の取得
	if (!GetKeyboardState(Key))
	{
		printf("キー情報の取得に失敗\n");
	}

	ZeroMemory(Map, sizeof(Map));
	ZeroMemory(Zangai, sizeof(Zangai));
	ZeroMemory(SaveZangai, sizeof(SaveZangai));
	ZeroMemory(SaveBlock, sizeof(SaveBlock));
	ZeroMemory(MoveBlock, sizeof(MoveBlock));

	// Mapの初期化
	// 上部5マスは画面外
	// 写っているのは10*20
	for (short i = 0; i < MAP_HEIGHT - 20; i++)
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			Map[i][j] = EChip::OutSide;
		}
	}

	// Zangaiの仮配置
	for (short i = MAP_HEIGHT - 10; i < MAP_HEIGHT; i++)
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			Zangai[i][0] = EChip::Exist;
			Zangai[i][7] = EChip::Exist;
			Zangai[i][8] = EChip::Exist;
			Zangai[i][9] = EChip::Exist;

			Map[i][j] = Zangai[i][j];
		}
	}

	// ブロック初期座標
	FirstPosition.X = FirstPosition.Y = 0;
	NewPosition.X = NewPosition.Y = 0;
	RotateDir = ERotationType::Standard;
	SaveRotateInfo = 4;
	EntryBlockNum = 0;

	LeftRotateFlag = FALSE;
	LeftMoveFlag = FALSE;
	RightMoveFlag = FALSE;
	PushDownFlag = FALSE;
	PushReturnFlag = FALSE;
	EndFlag = FALSE;

	// 最初に何らかの形のブロックを作る
	EntryBlock(MoveBlock);

	// 初期マップの描画
	Draw();

	return TRUE;
}

void EntryBlock(short Data[BLOCK_NUM][BLOCK_NUM])
{
	// 最初に空にする初期化
	ZeroMemory(Data, sizeof(MoveBlock));

	// ブロックの最大数の範囲で乱数生成
	short EntryNum = rand() % EBlockCategory::MaxNum;

	// デバッグ用
	EntryNum = EBlockCategory::Stick;

	// ブロック生成
	// [2][2]が中心とする
	switch (EntryNum)
	{
	case EBlockCategory::Square:	// 四角
		Data[1][1] = EChip::Exist;
		Data[1][2] = EChip::Exist;
		Data[2][1] = EChip::Exist;
		Data[2][2] = EChip::Exist;
		printf("形:四角\n");
		break;
	case EBlockCategory::Stick:		// 棒(縦または横)
		for (short i = 1; i <= 4; i++)
		{
			Data[i][2] = EChip::Exist;
		}
		printf("形:棒\n");
		break;
	case EBlockCategory::Convex:	// 凸
		Data[1][2] = EChip::Exist;
		Data[2][1] = EChip::Exist;
		Data[2][2] = EChip::Exist;
		Data[2][3] = EChip::Exist;
		printf("形:凸\n");
		break;
	case EBlockCategory::Z:			// Z
		Data[2][1] = EChip::Exist;
		Data[2][2] = EChip::Exist;
		Data[3][2] = EChip::Exist;
		Data[3][3] = EChip::Exist;
		printf("形:Z\n");
		break;
	case EBlockCategory::InvZ:		// 逆Z
		Data[2][2] = EChip::Exist;
		Data[2][3] = EChip::Exist;
		Data[3][1] = EChip::Exist;
		Data[3][2] = EChip::Exist;
		printf("形:逆Z\n");
		break;
	case EBlockCategory::L:			// L
		Data[1][1] = EChip::Exist;
		Data[2][1] = EChip::Exist;
		Data[2][2] = EChip::Exist;
		Data[2][3] = EChip::Exist;
		printf("形:L\n");
		break;
	case EBlockCategory::InvL:		// 逆L
		Data[1][3] = EChip::Exist;
		Data[2][1] = EChip::Exist;
		Data[2][2] = EChip::Exist;
		Data[2][3] = EChip::Exist;
		printf("形:逆L\n");
		break;
	default:
		printf("ブロックが作れませんでした\n");
		break;
	}
	printf("\n");

	// ブロック番号を保存
	EntryBlockNum = EntryNum;

	// ブロックのマップ初期座標
	RotateDir = 0;
	SaveRotateInfo = RotateDir;
	// ブロックは全て5×5の配列からなる
	FirstPosition = { 2, 4 };
	// 次回から使うポジションを最初のポジションにしておく
	NewPosition = FirstPosition;
	// マップに配置
	for (short i = 0; i < BLOCK_NUM; i++)
	{
		for (short j = 0; j < BLOCK_NUM; j++)
		{
			if (Data[i][j] == EChip::None)
				continue;

			Map[FirstPosition.Y + i][FirstPosition.X + j] = Data[i][j];
		}
	}
}

// 更新
void Update(COORD* BlockPosition, short RotateInfo, bool* End)
{
	// --- マップの更新 ---
	// Mapをまっさらな状態に
	for (short i = 0; i < MAP_HEIGHT; i++)
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			if (i < 5)
			{
				Map[i][j] = EChip::OutSide;
			}
			else
			{
				Map[i][j] = EChip::None;
			}
		}
	}

	// --- MoveBlockの座標更新 ---
	// 回転したかどうかチェック
	RotationCheck(BlockPosition, RotateInfo);

	// 座標補正
	switch (EntryBlockNum)
	{
	case EBlockCategory::Square:
		if (RotateInfo == ERotationType::Standard || RotateInfo == ERotationType::Left)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Right || RotateInfo == ERotationType::Inverse)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		break;
	case EBlockCategory::Stick:
		if (RotateInfo == ERotationType::Standard || RotateInfo == ERotationType::Inverse)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Right)
		{
			if (BlockPosition->X <= EOffsetCoord::Min)
			{
				BlockPosition->X = EOffsetCoord::Min;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Left)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::Max)
			{
				BlockPosition->X = EOffsetCoord::Max;
			}
		}
		break;
	case EBlockCategory::Convex:
		if (RotateInfo == ERotationType::Standard || RotateInfo == ERotationType::Inverse)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Right)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Left)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		break;
	case EBlockCategory::Z:
	case EBlockCategory::InvZ:
		if (RotateInfo == ERotationType::Standard || RotateInfo == ERotationType::Inverse)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Right)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Left)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		break;
	case EBlockCategory::L:
	case EBlockCategory::InvL:
		if (RotateInfo == ERotationType::Standard || RotateInfo == ERotationType::Inverse)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Right)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (RotateInfo == ERotationType::Left)
		{
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		break;
	default:
		break;
	}

	// 配置前にZangaiに触れているか/マップの下部にいるか確認
	// いるならMoveBlockの情報をZangaiに書き込み削除/初期位置に再生成
	bool MoveStopFlag = FALSE;

	// MoveBlockが一番下またはZangai[][] = Existに触れているかの処理
	// MoveBlockをMapに入れる前に行う
	for (short i = BLOCK_NUM - 1; i >= 0; i--)
	{
		if (BlockPosition->Y + i < 0 || BlockPosition->Y + i > MAP_HEIGHT)
			continue;

		for (short j = 0; j < BLOCK_NUM; j++)
		{
			// MoveBlock[i][j]がEChip::Existでなければ/配列外なら処理を飛ばす
			if (MoveBlock[i][j] != EChip::Exist ||
				BlockPosition->X + j < 0 || BlockPosition->X + j >= MAP_WIDTH)
				continue;

			// 下:MoveBlock内で一番下にあるブロック/MoveBlock一番下列がブロックの場合を見る
			if (PushReturnFlag && ((i < (BLOCK_NUM - 1) && MoveBlock[i + 1][j] == EChip::None) || (MoveBlock[BLOCK_NUM - 1][j] == EChip::Exist)))
			{
				// Zangai = EChip::Existだったら
				if (Zangai[BlockPosition->Y + i][BlockPosition->X + j] == EChip::Exist)
				{
					// めり込んでいる分戻す
					BlockPosition->Y--;
					// MoveBlockは止まる
					MoveStopFlag = TRUE;
				}

				// 一つ先を見る
				BlockPosition->Y++;
				// 一つ先が最下部だったら
				if (BlockPosition->Y + i >= MAP_HEIGHT)
				{
					// MoveBlockは止まる
					MoveStopFlag = TRUE;
				}
				// 戻す
				BlockPosition->Y--;
			}
			// 左:jが最小ではないかつMoveBlock内で一番左のブロック/MoveBlock一番左列がブロックの場合を見る
			else if (LeftMoveFlag && ((j > 0 && MoveBlock[i][j - 1] == EChip::None) || (MoveBlock[i][0] == EChip::Exist)) &&
				Zangai[BlockPosition->Y + i][BlockPosition->X + j] == EChip::Exist)
			{
				// 今いる座標にZangaiがあるか
				//if (Zangai[BlockPosition->Y + i][BlockPosition->X + j] == EChip::Exist)
				{
					// めり込んでいる分戻す
					BlockPosition->X++;
					// MoveBlockはまだ止まらない
					MoveStopFlag = FALSE;
				}
			}
			// 右:jが最大ではないかつMoveBlock内で一番右のブロック/MoveBlock一番右列がブロックの場合を見る
			else if (RightMoveFlag && ((j < (BLOCK_NUM - 1) && MoveBlock[i][j + 1] == EChip::None) || (MoveBlock[i][BLOCK_NUM - 1] == EChip::Exist)) &&
				Zangai[BlockPosition->Y + i][BlockPosition->X + j] == EChip::Exist)
			{
				{
					// めり込んでいる分戻す
					BlockPosition->X--;
					// MoveBlockはまだ止まらない
					MoveStopFlag = FALSE;
				}
			}
		}
	}

	// MoveBlockをMapかZangaiに書き込む
	for (short i = 0; i < BLOCK_NUM; i++)
	{
		for (short j = 0; j < BLOCK_NUM; j++)
		{
			// マップの配列外に出てしまうなら処理を飛ばす
			if (BlockPosition->Y + i < 0 || BlockPosition->Y + i > MAP_HEIGHT ||
				BlockPosition->X + j < 0 || BlockPosition->X + j > MAP_WIDTH)
				continue;

			if (!MoveStopFlag)
			{
				if (MoveBlock[i][j] == EChip::None)
					continue;

				// いなければMapに書き込み続行
				Map[BlockPosition->Y + i][BlockPosition->X + j] = MoveBlock[i][j];
			}
			else
			{
				if (MoveBlock[i][j] == EChip::None)
					continue;

				Zangai[BlockPosition->Y + i][BlockPosition->X + j] = MoveBlock[i][j];
			}
		}
	}

	short LastNumber = -1;
	short DisappNum = 0;
	// Zangaiのi番目が一列そろっているかの確認
	for (short i = 0; i < MAP_HEIGHT; i++)
	{
		// 最初見てみてブロックが置かれていなければそろわないので飛ばす
		if (Zangai[i][0] != EChip::Exist)
			continue;

		// 一列そろっているかの判定用カウント
		short AlignCount = -1;
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			// ブロックが置かれていなければそろってないので終了
			if (Zangai[i][j] != EChip::Exist)
				break;

			// ブロックの場合カウントを増やす
			AlignCount++;
			
			// 一列がブロックでそろっていた場合
			if (AlignCount == MAP_WIDTH - 1)
			{
				// Zangaiの一列を削除する処理
				while (AlignCount > -1)
				{
					Zangai[i][AlignCount] = EChip::None;
					AlignCount--;
				}
				LastNumber = i;
				DisappNum++;
			}
		}
	}

	// numが変わっている場合消えた分配列をずらす
	if (LastNumber != -1)
	{
		for (short i = LastNumber; i >= 0; i--)
		{
			for (short j = 0; j < MAP_WIDTH; j++)
			{
				if (i - 1 < 0)
					break;

				SaveZangai[i][j] = Zangai[i - DisappNum][j];
			}
		}
		// 改めてZangaiに配置
		for (short i = 0; i < LastNumber + 1; i++)
		{
			for (short j = 0; j < MAP_WIDTH; j++)
			{
				Zangai[i][j] = SaveZangai[i][j];
			}
		}
	}

	// Zangaiの再配置/ゲームオーバーの判定
	// 下からしゅっしゅっ
	for (short i = MAP_HEIGHT - 1; i >= 0; i--)
	{
		if (*End)
			break;

		for (short j = 0; j < MAP_WIDTH; j++)
		{
			// ブロックが置かれていなければ配置処理を飛ばす
			if (Zangai[i][j] != EChip::Exist)
				continue;

			if (Map[i][j] == EChip::OutSide && Zangai[i][j] == EChip::Exist)
			{ 
				*End = true;
				break;
			}
			Map[i][j] = Zangai[i][j];
		}
	}

	// ゲームオーバーになっておらず、MoveBlockが動けなければ
	if (!*End && MoveStopFlag)
	{
		// 新しく生成
		EntryBlock(MoveBlock);
	}

	// 入力関係を初期化
	LeftRotateFlag = FALSE;
	PushReturnFlag = FALSE;
	LeftMoveFlag = FALSE;
	RightMoveFlag = FALSE;

	// 最終的なMapを描画
	Draw();
}

// 回転したかどうかチェック
void RotationCheck(COORD* BlockPosition, int32_t RotateInfo)
{
	// ブロックの種類によって端まで行く際の座標が決まる

	// 前回の回転値が同じなら処理を飛ばす
	if (SaveRotateInfo == RotateInfo)
		return;

	// 左回転か右回転か
	if (!LeftRotateFlag)
	{
		for (int32_t i = BLOCK_NUM; i > 0; i--)
		{
			for (int32_t j = 0; j < BLOCK_NUM; j++)
			{
				// 00 10 20 30 40 = 40 41 42 43 44
				// 01 11 21 31 41 = 30 31 32 33 34
				// 02 12 22 32 42 = 20 21 22 23 24
				// 03 13 23 33 43 = 10 11 12 13 14
				// 04 14 24 34 44 = 00 01 02 03 04
				SaveBlock[j][BLOCK_NUM - i] = MoveBlock[i - 1][j];
			}
		}
	}
	else
	{
		for (int32_t i = 0; i < BLOCK_NUM; i++)
		{
			for (int32_t j = BLOCK_NUM; j > 0; j--)
			{
				// 00 10 20 30 40 = 04 03 02 01 00
				// 01 11 21 31 41 = 14 13 12 11 10
				// 02 12 22 32 42 = 24 23 22 21 20
				// 03 13 23 33 43 = 34 33 32 31 30
				// 04 14 24 34 44 = 44 43 42 41 40
				SaveBlock[BLOCK_NUM - j][i] = MoveBlock[i][j - 1];
			}
		}
	}
	memcpy(MoveBlock, SaveBlock, sizeof(short) * BLOCK_NUM * BLOCK_NUM);

	// 次からのスキップ用に回転情報を保存
	SaveRotateInfo = RotateInfo;
}

// 描画
void Draw()
{
	// マップの描画
	for (short i = 0; i < MAP_HEIGHT; i++)
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			switch (Map[i][j])
			{
			case EChip::None:
				// 図形の表示
				printf("□");
				break;
			case EChip::Exist:
				// 図形の表示
				printf("■");
				break;
			case EChip::OutSide:
				printf("×");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
	printf("\n");

	printf("「A」:左移動 「D」:右移動 「Enter」キーで更新\n\n");
}