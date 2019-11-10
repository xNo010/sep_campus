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
		// 左・右回り回転:←→キー
		if (!PushRotateFlag && GetKeyState(VK_LEFT) & 0x80)
		{
			printf("左回りに回転\n");
			RotateDir--;
			RotateDir %= 4;
			printf("回転値:%d\n", RotateDir);
			PushRotateFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}
		else if (!PushRotateFlag && GetKeyState(VK_RIGHT) & 0x80)
		{
			printf("右回りに回転\n");
			RotateDir++;
			RotateDir %= 4;
			printf("回転値:%d\n", RotateDir);
			PushRotateFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}

		// 左右移動:ADキー
		if (!PushLeftRightFlag && GetKeyState('A') & 0x80)
		{
			printf("左に1マス移動\n");
			NewPosition.X -= MovingValue;
			PushLeftRightFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}
		else if (!PushLeftRightFlag && GetKeyState('D') & 0x80)
		{
			printf("右に1マス移動\n");
			NewPosition.X += MovingValue;
			PushLeftRightFlag = TRUE;

			// 更新
			Update(&NewPosition, RotateDir, EndFlag);

			// 通常の待機時間
			Sleep(300);
			//Sleep(1000);
			continue;
		}

		// 高速落下
		if (!PushDownFlag && GetKeyState('S') & 0x80)
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
			Update(&NewPosition, RotateDir, EndFlag);

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
	RotateDir = 0;
	SaveRotate = 4;
	EntryBlockNum = 0;

	PushRotateFlag = FALSE;
	PushLeftRightFlag = FALSE;
	PushDownFlag = FALSE;
	PushReturnFlag = FALSE;
	EndFlag = FALSE;

	// 最初に何らかの形のブロックを作る
	EntryBlock(MoveBlock);

	// 初期マップの描画
	Draw();

	return TRUE;
}

void EntryBlock(short BlockData[][BLOCK_NUM])
{
	// 最初に空にする初期化
	ZeroMemory(BlockData, sizeof(MoveBlock));

	// ブロックの最大数の範囲で乱数生成
	short EntryNum = rand() % EBlockCategory::MaxNum;

	// デバッグ用
	EntryNum = EBlockCategory::Square;

	// ブロック生成
	// [2][2]が中心
	switch (EntryNum)
	{
	case EBlockCategory::Square:	// 四角
		BlockData[1][1] = EChip::Exist;
		BlockData[1][2] = EChip::Exist;
		BlockData[2][1] = EChip::Exist;
		BlockData[2][2] = EChip::Exist;
		printf("形:四角\n");
		break;
	case EBlockCategory::Stick:		// 棒(縦または横)
		for (short i = 1; i <= 4; i++)
		{
			BlockData[i][2] = EChip::Exist;
		}
		printf("形:棒\n");
		break;
	case EBlockCategory::Convex:	// 凸
		BlockData[1][2] = EChip::Exist;
		BlockData[2][1] = EChip::Exist;
		BlockData[2][2] = EChip::Exist;
		BlockData[2][3] = EChip::Exist;
		printf("形:凸\n");
		break;
	case EBlockCategory::Z:			// Z
		BlockData[2][1] = EChip::Exist;
		BlockData[2][2] = EChip::Exist;
		BlockData[3][2] = EChip::Exist;
		BlockData[3][3] = EChip::Exist;
		printf("形:Z\n");
		break;
	case EBlockCategory::InvZ:		// 逆Z
		BlockData[2][2] = EChip::Exist;
		BlockData[2][3] = EChip::Exist;
		BlockData[3][1] = EChip::Exist;
		BlockData[3][2] = EChip::Exist;
		printf("形:逆Z\n");
		break;
	case EBlockCategory::L:			// L
		BlockData[1][1] = EChip::Exist;
		BlockData[2][1] = EChip::Exist;
		BlockData[2][2] = EChip::Exist;
		BlockData[2][3] = EChip::Exist;
		printf("形:L\n");
		break;
	case EBlockCategory::InvL:		// 逆L
		BlockData[1][3] = EChip::Exist;
		BlockData[2][1] = EChip::Exist;
		BlockData[2][2] = EChip::Exist;
		BlockData[2][3] = EChip::Exist;
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
	// ブロックは全て5×5の配列からなる
	FirstPosition = { 2, 4 };
	NewPosition = FirstPosition;
	// マップに配置
	for (short i = 0; i < BLOCK_NUM; i++)
	{
		for (short j = 0; j < BLOCK_NUM; j++)
		{
			if (BlockData[i][j] == EChip::None)
				continue;

			Map[FirstPosition.Y + i][FirstPosition.X + j] = BlockData[i][j];
		}
	}
}

// 更新
void Update(COORD* BlockPosition, short Rotate, bool End)
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

	// MoveBlockの座標更新
	// 先に回転処理
	// 0の時・-1(3)の時・1(-3)の時・2(-2)の時が存在
	// ブロックの種類によって端まで行く際の座標が決まる
	switch (EntryBlockNum)
	{
	case EBlockCategory::Square:	// 四角
		// 移動制限
		if (Rotate == -1 || Rotate == 0 || Rotate == 3)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		else if (Rotate == -3 || Rotate == 1 || abs(Rotate) == 2)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		
		// 回転値同じなら回転処理は不要
		if (SaveRotate == Rotate)
			break;
		// 回転による形状変化
		if (Rotate == 0)
		{
			MoveBlock[1][1] = EChip::Exist;
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;

			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][1] = EChip::None;
			MoveBlock[3][2] = EChip::None;
		}
		else if (RotateDir == -3 || RotateDir == 1)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[1][3] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[3][2] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][1] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[1][2] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (abs(RotateDir) == 2)
		{
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;
			MoveBlock[3][3] = EChip::Exist;

			MoveBlock[1][2] = EChip::None;
			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[3][1] = EChip::None;
		}

		break;
	case EBlockCategory::Stick:		// 棒(縦または横)
		// 移動制限
		if (Rotate == 0 || abs(Rotate) == 2)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::Min)
			{
				BlockPosition->X = EOffsetCoord::Min;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::Max)
			{
				BlockPosition->X = EOffsetCoord::Max;
			}
		}

		// 回転値同じなら回転処理は不要
		if (SaveRotate == Rotate)
			break;
		// 回転による形状変化
		if (Rotate == 0)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;
			MoveBlock[4][2] = EChip::Exist;

			MoveBlock[2][0] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[2][4] = EChip::None;
		}
		else if (RotateDir == -3 || RotateDir == 1)
		{
			MoveBlock[2][0] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;

			MoveBlock[0][2] = EChip::None;
			MoveBlock[1][2] = EChip::None;
			MoveBlock[3][2] = EChip::None;
			MoveBlock[4][2] = EChip::None;
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[2][4] = EChip::Exist;

			MoveBlock[0][2] = EChip::None;
			MoveBlock[1][2] = EChip::None;
			MoveBlock[3][2] = EChip::None;
			MoveBlock[4][2] = EChip::None;
		}
		else if (abs(RotateDir) == 2)
		{
			MoveBlock[0][2] = EChip::Exist;
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[2][0] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[2][4] = EChip::None;
		}
		break;
	case EBlockCategory::Convex:	// 凸
		// 移動制限
		if (Rotate == 0 || abs(Rotate) == 2)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		
		// 回転値同じなら回転処理は不要
		if (SaveRotate == Rotate)
			break;
		// 回転による形状変化
		if (Rotate == 0)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;

			MoveBlock[3][2] = EChip::None;
		}
		else if (RotateDir == -3 || RotateDir == 1)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[2][1] = EChip::None;
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[2][3] = EChip::None;
		}
		else if (abs(RotateDir) == 2)
		{
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][2] = EChip::None;
		}

		break;
	case EBlockCategory::Z:			// Z
		// 移動制限
		if (Rotate == 0 || abs(Rotate) == 2)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}

		// 回転値同じなら回転処理は不要
		if (SaveRotate == Rotate)
			break;
		// 回転による形状変化
		if (Rotate == 0)
		{
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;
			MoveBlock[3][3] = EChip::Exist;

			MoveBlock[1][2] = EChip::None;
			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][1] = EChip::None;
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][1] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][2] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			MoveBlock[1][3] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[1][2] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[3][1] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (abs(Rotate) == 2)
		{
			MoveBlock[1][1] = EChip::Exist;
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;

			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[3][1] = EChip::None;
			MoveBlock[3][2] = EChip::None;
		}
		break;
	case EBlockCategory::InvZ:		// 逆Z
		// 移動制限
		if (Rotate == 0 || abs(Rotate) == 2)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}

		// 回転値同じなら回転処理は不要
		if (SaveRotate == Rotate)
			break;
		// 回転による形状変化
		if (Rotate == 0)
		{
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][1] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[1][2] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			MoveBlock[1][1] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][2] = EChip::None;
			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][1] = EChip::None;
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][3] = EChip::Exist;

			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[3][1] = EChip::None;
			MoveBlock[3][2] = EChip::None;
		}
		else if (abs(Rotate) == 2)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[1][3] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][2] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		break;
	case EBlockCategory::L:			// L
		// 移動制限
		if (Rotate == 0 || abs(Rotate) == 2)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}

		// 回転値同じなら回転処理は不要
		if (SaveRotate == Rotate)
			break;
		// 回転による形状変化
		if (Rotate == 0)
		{
			MoveBlock[1][1] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;

			MoveBlock[1][2] = EChip::None;
			MoveBlock[1][3] = EChip::None;
			MoveBlock[3][1] = EChip::None;
			MoveBlock[3][2] = EChip::None;
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[1][3] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][1] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (abs(Rotate) == 2)
		{
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][3] = EChip::Exist;

			MoveBlock[1][2] = EChip::None;
			MoveBlock[1][3] = EChip::None;
			MoveBlock[3][1] = EChip::None;
			MoveBlock[3][2] = EChip::None;
		}
		break;
	case EBlockCategory::InvL:		// 逆L
		// 移動制限
		if (Rotate == 0 || abs(Rotate) == 2)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMax;
			}
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			// 左限界値
			if (BlockPosition->X <= EOffsetCoord::MoreMin)
			{
				BlockPosition->X = EOffsetCoord::MoreMin;
			}
			// 右限界値
			else if (BlockPosition->X >= EOffsetCoord::MoreMoreMax)
			{
				BlockPosition->X = EOffsetCoord::MoreMoreMax;
			}
		}

		// 回転値同じなら回転処理は不要
		if (SaveRotate == Rotate)
			break;
		// 回転による形状変化
		if (Rotate == 0)
		{
			MoveBlock[1][3] = EChip::Exist;
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[1][2] = EChip::None;
			MoveBlock[3][1] = EChip::None;
			MoveBlock[3][2] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		else if (Rotate == -3 || Rotate == 1)
		{
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;
			MoveBlock[3][3] = EChip::Exist;

			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][1] = EChip::None;
		}
		else if (Rotate == -1 || Rotate == 3)
		{
			MoveBlock[1][1] = EChip::Exist;
			MoveBlock[1][2] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[3][2] = EChip::Exist;

			MoveBlock[1][3] = EChip::None;
			MoveBlock[2][1] = EChip::None;
			MoveBlock[2][3] = EChip::None;
			MoveBlock[3][1] = EChip::None;
		}
		else if (abs(Rotate) == 2)
		{
			MoveBlock[2][1] = EChip::Exist;
			MoveBlock[2][2] = EChip::Exist;
			MoveBlock[2][3] = EChip::Exist;
			MoveBlock[3][1] = EChip::Exist;

			MoveBlock[1][1] = EChip::None;
			MoveBlock[1][2] = EChip::None;
			MoveBlock[3][2] = EChip::None;
			MoveBlock[3][3] = EChip::None;
		}
		break;
	default:
		break;
	}
	// 次からのスキップ用に回転情報を保存
	SaveRotate = Rotate;

	// 配置前にZangaiに触れているか/マップの下部にいるか確認
	// いるならMoveBlockの情報をZangaiに書き込み削除/初期位置に再生成
	bool MoveStopFlag = FALSE;

	// MoveBlockが一番下またはZangai[][] = Existに触れているかの処理
	// MoveBlockをMapに入れる前に行う
	for (short i = BLOCK_NUM - 1; i >= 0; i--)
	{
		if (BlockPosition->Y + i < 0 || BlockPosition->Y + i >= MAP_HEIGHT)
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
			else if (PushLeftRightFlag && ((j > 0 && MoveBlock[i][j - 1] == EChip::None) || (MoveBlock[i][0] == EChip::Exist)) &&
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
			//else if (PushLeftRightFlag && ((j < (BLOCK_NUM - 1) && MoveBlock[i][j + 1] == EChip::None) || (MoveBlock[i][BLOCK_NUM - 1] == EChip::Exist)) &&
			//	Zangai[BlockPosition->Y + i][BlockPosition->X + j] == EChip::Exist)
			//{
			//	{
			//		// めり込んでいる分戻す
			//		BlockPosition->X--;
			//		// MoveBlockはまだ止まらない
			//		MoveStopFlag = FALSE;
			//	}
			//}
			else
			{
				// 何もせずfor文を終了
				break;
			}
		}
	}

	// MoveBlockをMapかZangaiに書き込む
	for (short i = 0; i < BLOCK_NUM; i++)
	{
		for (short j = 0; j < BLOCK_NUM; j++)
		{
			// マップの配列外に出てしまうなら処理を飛ばす
			if (BlockPosition->Y + i < 0 || BlockPosition->Y + i >= MAP_HEIGHT ||
				BlockPosition->X + j < 0 || BlockPosition->X + j >= MAP_WIDTH)
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
		// 一列そろっているかの判定用カウント
		short AlignCount = -1;
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			// ブロックが置かれていなければ配置処理を飛ばす
			if (Zangai[i][j] != EChip::Exist)
				continue;

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

	// Zangaiの再配置
	// 下からしゅっしゅっ
	for (short i = MAP_HEIGHT - 1; i >= 0; i--)
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			// ブロックが置かれていなければ配置処理を飛ばす
			if (Zangai[i][j] != EChip::Exist)
				continue;

			Map[i][j] = Zangai[i][j];
		}
	}

	// MoveBlockが動けなければ
	if (MoveStopFlag)
	{
		// 新しく生成
		EntryBlock(MoveBlock);
	}

	// 入力関係を初期化
	PushRotateFlag = FALSE;
	PushReturnFlag = FALSE;
	PushLeftRightFlag = FALSE;

	// 最終的なMapを描画
	Draw();
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