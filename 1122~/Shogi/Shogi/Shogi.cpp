#include "Shogi.h"

int main()
{
	Initialize();

	// 最初の表示
	Draw(ShogiBoard);
	CapPieceDraw(CapturedPieceNum);

	while (1)
	{
		// 現在の手を表示
		NowHandDisp(NowHand);

		// 持ち駒の使用有無(使わないと選択した以降は呼ばない)
		if (IsUseCapFunc || !IsUseCapPiece(CapturedPieceNum, NowHand))
		{
			// 駒がある座標・駒を動かす座標を指定
			if (!InputPos_IsPiece(ShogiBoard, NowHand) || !InputPos_IsMovePiece(ShogiBoard, NowHand))
			{
				// falseならやり直し
				printf("\n");
				continue;
			}
		}

		Update(NowHand);
		printf("\n");
	}
}

void Initialize()
{
	HandNum = 0;
	NowHand = HandNum % 2;
	IsBack = IsUseCapFunc = false;
	BackSavePiece = EPiece::None;
	ZeroMemory(SelectPiecePos, sizeof(SelectPiecePos));
	ZeroMemory(MoveInputPos, sizeof(MoveInputPos));
	ZeroMemory(KingPos, sizeof(KingPos));
	ZeroMemory(CapturedPieceNum, sizeof(CapturedPieceNum));
	ZeroMemory(SaveRecord, sizeof(SaveRecord));
	ZeroMemory(ShogiBoard, sizeof(ShogiBoard));

	// 駒をマップに配置
	const int32_t Center = 4;
	const int32_t ThreeCell = 3;
	int32_t OwnPieceInfo = EPiece::Own_King;
	int32_t EnemyPieceInfo = EPiece::Enemy_King;
	// 自陣・敵陣に駒配置
	for (int32_t i = 0; i < VERT_NUM / 3; i++)
	{
		int32_t LeftShift = Center, RightShift = Center;
		switch (i)
		{
		case 0:
			// 玉/王
			ShogiBoard[i][Center] = EnemyPieceInfo;
			KingPos[EHand::Second] = { Center, i };
			ShogiBoard[VERT_NUM - i - 1][Center] = OwnPieceInfo;
			KingPos[EHand::First] = { Center, VERT_NUM - i - 1 };

			OwnPieceInfo = EPiece::None;
			EnemyPieceInfo = EPiece::Enemy_Gold - 1;
			// 金～香
			while (LeftShift != 0 && RightShift != HORI_NUM - 1)
			{
				ShogiBoard[i][--LeftShift] = ++EnemyPieceInfo;
				ShogiBoard[i][++RightShift] = EnemyPieceInfo;
				ShogiBoard[VERT_NUM - i - 1][LeftShift] = ++OwnPieceInfo;
				ShogiBoard[VERT_NUM - i - 1][RightShift] = OwnPieceInfo;
			}
			break;
		case 1:
			LeftShift -= ThreeCell;
			RightShift += ThreeCell;

			// 角
			ShogiBoard[i][RightShift] = ++EnemyPieceInfo;
			ShogiBoard[VERT_NUM - i - 1][LeftShift] = ++OwnPieceInfo;

			// 飛
			ShogiBoard[i][LeftShift] = ++EnemyPieceInfo;
			ShogiBoard[VERT_NUM - i - 1][RightShift] = ++OwnPieceInfo;
			break;
		case 2:
			// 歩
			OwnPieceInfo++, EnemyPieceInfo++;
			for (int32_t j = 0; j < HORI_NUM; j++)
			{
				ShogiBoard[i][j] = EnemyPieceInfo;
				ShogiBoard[VERT_NUM - i - 1][j] = OwnPieceInfo;
			}
			break;
		default:
			break;
		}
	}
}

bool InputFunc(POSITION* pPos)
{
	ZeroMemory(pPos, sizeof(POSITION));
	printf("列の選択(整数で入力):");
	scanf_s("%d", &pPos->x);
	// 将棋盤の外に行ってしまったかのチェック
	if (pPos->x <= 0 || HORI_NUM < pPos->x)
	{
		rewind(stdin);
		return false;
	}
	printf("段の選択(整数で入力):");
	scanf_s("%d", &pPos->y);
	if (pPos->y <= 0 || VERT_NUM < pPos->y)
	{
		rewind(stdin);
		return false;
	}

	return true;
}

// 列数値の配列用⇔表示用変換
void HoriConv(int32_t* HoriPos)
{
	*HoriPos = HORI_NUM + 1 - *HoriPos;
}

// 駒があるかどうかのチェック
bool InputPos_IsPiece(int32_t ShogiBoard[VERT_NUM][HORI_NUM], bool Hand)
{
	POSITION InputPos;

	printf("・動かす駒の選択\n");
	// 入力に失敗したらやり直し
	if (!InputFunc(&InputPos))
	{
		printf("そこは盤の外です\n");
		return false;
	}

	// 列に関しては、右から左で1～9となるので、配列用に値を変換
	HoriConv(&InputPos.x);

	// 駒がなければやり直し
	if (ShogiBoard[--InputPos.y][--InputPos.x] == EPiece::None)
	{
		printf("そこに駒はありません\n");
		return false;
	}

	// 駒はあるが、自分の駒でなければやり直し
	if (!Hand)
	{
		// 選択座標にある駒情報のIDが、自身の駒番号より大きい
		if (ShogiBoard[InputPos.y][InputPos.x] > EPiece::Own_PromRook)
		{
			printf("自身の駒ではありません\n");
			return false;
		}
	}
	else
	{
		// 選択座標にある駒情報のIDが、自身の駒番号より小さい
		if (ShogiBoard[InputPos.y][InputPos.x] < EPiece::Own_PromRook)
		{
			printf("自身の駒ではありません\n");
			return false;
		}
	}

	// 駒情報を入れる
	InputRecord.Piece = ShogiBoard[InputPos.y][InputPos.x];

	// 入力情報をグローバルに保存
	SelectPiecePos[Hand] = InputPos;

	return true;
}

bool InputPos_IsMovePiece(int32_t ShogiBoard[VERT_NUM][HORI_NUM], bool Hand)
{
	POSITION InputPos;

	printf("・駒を動かす座標選択\n");
	// 入力に失敗したらやり直し
	if (!InputFunc(&InputPos))
	{
		printf("そこは盤の外です\n");
		return false;
	}

	// 列に関しては、右から左で1～9となるので、配列用に値を変換
	HoriConv(&InputPos.x);

	// 判定用に更新
	InputPos.x--, InputPos.y--;

	// 同じマスを選んでいたらやり直し
	if (SelectPiecePos[Hand] == InputPos)
	{
		printf("同じマスは選べません\n");
		return false;
	}

	// 駒座標と移動座標が何マス分離れているか
	POSITION DiffCell = SelectPiecePos[Hand] - InputPos;

	// 先手後手で判定は別
	if (!Hand)
	{
		// 2マス以上進もうとした際、一度に2マス以上進めない駒だったらここでやり直し
		if ((abs(DiffCell.x) > 1 || abs(DiffCell.y) > 1) &&
			(InputRecord.Piece <= EPiece::Own_Silver || InputRecord.Piece == EPiece::Own_PromPawn ||
			(InputRecord.Piece >= EPiece::Own_Pawn && EPiece::Own_PromLance >= InputRecord.Piece)))
		{
			printf("選択した駒は移動できません\n");
			return false;
		}
	}
	else
	{
		if ((abs(DiffCell.x) > 1 || abs(DiffCell.y) > 1) &&
			(InputRecord.Piece <= EPiece::Enemy_Silver || InputRecord.Piece == EPiece::Enemy_PromPawn ||
			(InputRecord.Piece >= EPiece::Enemy_Pawn && EPiece::Enemy_PromLance >= InputRecord.Piece)))
		{
			printf("選択した駒は移動できません\n");
			return false;
		}
	}

	// 選択駒が移動可能な座標かチェック
	const int32_t OneCell = 1;
	const int32_t TwoCell = 2;
	// 駒によってfalse条件が変わる
	switch (InputRecord.Piece)
	{
	case EPiece::Own_Gold:
	case EPiece::Own_PromSilver:
	case EPiece::Own_PromKnight:
	case EPiece::Own_PromLance:
	case EPiece::Own_PromPawn:
		if (DiffCell.y == +OneCell && (abs(DiffCell.x) == OneCell))
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Enemy_Gold:
	case EPiece::Enemy_PromSilver:
	case EPiece::Enemy_PromKnight:
	case EPiece::Enemy_PromLance:
	case EPiece::Enemy_PromPawn:
		if (DiffCell.y == -OneCell && (abs(DiffCell.x) == OneCell))
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Own_Silver:
		if ((DiffCell.x == 0 && DiffCell.y == -OneCell) || (abs(DiffCell.x) == OneCell && DiffCell.y == 0))
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Enemy_Silver:
		if ((DiffCell.x == 0 && DiffCell.y == +OneCell) || (abs(DiffCell.x) == OneCell && DiffCell.y == 0))
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Own_Knight:
		if (abs(DiffCell.x) == 0 || DiffCell.y != TwoCell)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Enemy_Knight:
		if (abs(DiffCell.x) == 0 || DiffCell.y != -TwoCell)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Own_Lance:
		if (abs(DiffCell.x) != 0 || DiffCell.y < 0)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		// 進路検索
		for (int32_t i = SelectPiecePos[Hand].y - 1; i > InputPos.y; i--)
		{
			if (ShogiBoard[i][InputPos.x] != EPiece::None)
			{
				printf("駒を飛び越すことはできません\n");
				return false;
			}
		}
		break;
	case EPiece::Enemy_Lance:
		if (abs(DiffCell.x) != 0 || DiffCell.y > 0)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		// 進路検索
		for (int32_t i = SelectPiecePos[Hand].y + 1; i < InputPos.y; i++)
		{
			if (ShogiBoard[i][InputPos.x] != EPiece::None)
			{
				printf("駒を飛び越すことはできません\n");
				return false;
			}
		}
		break;
	case EPiece::Own_Bishop:
	case EPiece::Enemy_Bishop:
		// true条件
		if (abs(DiffCell.x) - abs(DiffCell.y) == 0)
		{
			// 進路検索の分岐
			if (DiffCell.y > 0)
			{
				if (DiffCell.x > 0)
				{
					// 斜め左上で検索
					for (int32_t i = SelectPiecePos[Hand].y - 1, j = SelectPiecePos[Hand].x - 1; i > InputPos.y&& j > InputPos.x; i--, j--)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
				else
				{
					// 斜め右上で検索
					for (int32_t i = SelectPiecePos[Hand].y - 1, j = SelectPiecePos[Hand].x + 1; i > InputPos.y&& j < InputPos.x; i--, j++)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
			}
			else
			{
				if (DiffCell.x > 0)
				{
					// 斜め左下で検索
					for (int32_t i = SelectPiecePos[Hand].y + 1, j = SelectPiecePos[Hand].x - 1; i < InputPos.y && j > InputPos.x; i++, j--)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
				else
				{
					// 斜め右下で検索
					for (int32_t i = SelectPiecePos[Hand].y + 1, j = SelectPiecePos[Hand].x + 1; i < InputPos.y && j < InputPos.x; i++, j++)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
			}
			// switch文を抜ける
			break;
		}
		printf("選択した駒はそこに移動できません\n");
		return false;
		break;
	case EPiece::Own_PromBishop:
	case EPiece::Enemy_PromBishop:
		// true条件
		if (abs(DiffCell.x) - abs(DiffCell.y) == 0)
		{
			// 進路検索の分岐
			if (DiffCell.y > 0)
			{
				if (DiffCell.x > 0)
				{
					// 斜め左上で検索
					for (int32_t i = SelectPiecePos[Hand].y - 1, j = SelectPiecePos[Hand].x - 1; i > InputPos.y&& j > InputPos.x; i--, j--)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
				else
				{
					// 斜め右上で検索
					for (int32_t i = SelectPiecePos[Hand].y - 1, j = SelectPiecePos[Hand].x + 1; i > InputPos.y&& j < InputPos.x; i--, j++)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
			}
			else
			{
				if (DiffCell.x > 0)
				{
					// 斜め左下で検索
					for (int32_t i = SelectPiecePos[Hand].y + 1, j = SelectPiecePos[Hand].x - 1; i < InputPos.y && j > InputPos.x; i++, j--)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
				else
				{
					// 斜め右下で検索
					for (int32_t i = SelectPiecePos[Hand].y + 1, j = SelectPiecePos[Hand].x + 1; i < InputPos.y && j < InputPos.x; i++, j++)
					{
						if (ShogiBoard[i][j] != EPiece::None)
						{
							printf("駒を飛び越すことはできません\n");
							return false;
						}
					}
				}
			}
			// switch文を抜ける
			break;
		}
		if (DiffCell.x != 0 && abs(DiffCell.y) == 1 ||
			DiffCell.y != 0 && abs(DiffCell.x) == 1)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Own_Rook:
	case EPiece::Enemy_Rook:
		// true条件
		if (abs(DiffCell.x) > 0 && DiffCell.y == 0 || 
			abs(DiffCell.y) > 0 && DiffCell.x == 0)
		{
			// 進路検索の分岐
			if (DiffCell.x < 0)
			{
				// 右方向検索
				for (int32_t i = SelectPiecePos[Hand].x + 1; i < InputPos.x; i++)
				{
					if (ShogiBoard[InputPos.y][i] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			else if (DiffCell.x > 0)
			{
				// 左方向検索
				for (int32_t i = SelectPiecePos[Hand].x - 1; i > InputPos.x; i--)
				{
					if (ShogiBoard[InputPos.y][i] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			else if (DiffCell.y > 0)
			{
				// 上方向検索
				for (int32_t i = SelectPiecePos[Hand].y - 1; i > InputPos.y; i--)
				{
					if (ShogiBoard[i][InputPos.x] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			else if (DiffCell.y < 0)
			{
				// 下方向検索
				for (int32_t i = SelectPiecePos[Hand].y + 1; i < InputPos.y; i++)
				{
					if (ShogiBoard[i][InputPos.x] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			// switch文を抜ける
			break;
		}
		printf("選択した駒はそこに移動できません\n");
		return false;
		break;
	case EPiece::Own_PromRook:
	case EPiece::Enemy_PromRook:
		// true条件
		if (abs(DiffCell.x) > 0 && DiffCell.y == 0 ||
			abs(DiffCell.y) > 0 && DiffCell.x == 0)
		{
			if (DiffCell.x < 0)
			{
				// 右方向検索
				for (int32_t i = SelectPiecePos[Hand].x + 1; i < InputPos.x; i++)
				{
					if (ShogiBoard[InputPos.y][i] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			else if (DiffCell.x > 0)
			{
				// 左方向検索
				for (int32_t i = SelectPiecePos[Hand].x - 1; i > InputPos.x; i--)
				{
					if (ShogiBoard[InputPos.y][i] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			else if (DiffCell.y > 0)
			{
				// 上方向検索
				for (int32_t i = SelectPiecePos[Hand].y - 1; i > InputPos.y; i--)
				{
					if (ShogiBoard[i][InputPos.x] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			else if (DiffCell.y < 0)
			{
				// 下方向検索
				for (int32_t i = SelectPiecePos[Hand].y + 1; i < InputPos.y; i++)
				{
					if (ShogiBoard[i][InputPos.x] != EPiece::None)
					{
						printf("駒を飛び越すことはできません\n");
						return false;
					}
				}
			}
			break;
		}
		if (abs(DiffCell.x) == 1 && abs(DiffCell.y) == 1)
		{
			break;
		}
		printf("選択した駒はそこに移動できません\n");
		return false;
		break;
	case EPiece::Own_Pawn:
		if (DiffCell.x != 0 || DiffCell.y != +OneCell)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Enemy_Pawn:
		if (DiffCell.x != 0 || DiffCell.y != -OneCell)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	default:
		break;
	}

	// 自分の駒があった場合、やり直し
	if (!CapPieceConfirm(InputPos, Hand))
	{
		return false;
	}

	// 入力情報をグローバルに保存
	MoveInputPos[Hand] = InputPos;

	return true;
}

void Update(bool Hand)
{
	// 最初に待ったをするかのチェック
	// 前の手で待ったを選ばなかった場合、もう待ったはさせない
	if (!IsBack && BackCheck(Hand))
	{
		// 今の手と逆の手の情報を書きかえる
		bool OppoHand = !Hand;
		// その手で成っていたら戻す
		int32_t PromCheck = ShogiBoard[MoveInputPos[OppoHand].y][MoveInputPos[OppoHand].x];
		if ((PromCheck >= EPiece::Own_PromSilver && EPiece::Own_PromPawn >= PromCheck) ||
			(PromCheck >= EPiece::Enemy_PromSilver && EPiece::Enemy_PromPawn >= PromCheck))
		{
			PromCheck -= PROM_DIFF;
		}
		// 元いた位置・元の駒に戻す
		ShogiBoard[SelectPiecePos[OppoHand].y][SelectPiecePos[OppoHand].x] = PromCheck;
		// 保存した駒情報を入れる
		if (!Hand)
		{
			ShogiBoard[MoveInputPos[OppoHand].y][MoveInputPos[OppoHand].x] = BackSavePiece + (EPiece::Enemy_Gold - 1);
			if (PromCheck == EPiece::Own_King)
			{
				KingPos[OppoHand] = MoveInputPos[OppoHand];
			}
		}
		else
		{
			ShogiBoard[MoveInputPos[OppoHand].y][MoveInputPos[OppoHand].x] = BackSavePiece;
			if (PromCheck == EPiece::Enemy_King)
			{
				KingPos[OppoHand] = MoveInputPos[OppoHand];
			}
		}
		// 戻した手で持ち駒を獲得していたなら、消す
		if (CapturedPieceNum[OppoHand][BackSavePiece] != ECapPiece::NoneCap)
		{
			CapturedPieceNum[OppoHand][BackSavePiece]--;
		}

		// 盤描画
		Draw(ShogiBoard);

		// 持ち駒の表示
		CapPieceDraw(CapturedPieceNum);

		// 総手数数を一つ戻す
		HandNum--;
		// 手数が0でなければ棋譜描画
		if (HandNum != 0)
		{
			RecordsDraw(InputRecord, Hand, IsBack);
		}

		// 今の手と逆にする
		NowHand = OppoHand;

		// 持ち駒関数を呼んだ情報をリセット
		IsUseCapFunc = false;

		// 待ったの処理をしたので終了
		return;
	}
	else
	{
		IsBack = false;
	}

	// 先手か後手かの情報 + 移動座標を入れる
	InputRecord.Hand = Hand;
	InputRecord.SelectPos = SelectPiecePos[Hand];
	InputRecord.MovePos = MoveInputPos[Hand];

	// 入力情報を参考に値の入れ替え
	ShogiBoard[InputRecord.MovePos.y][InputRecord.MovePos.x] = InputRecord.Piece;
	// 王を移動させたなら、王の座標を更新
	if (!Hand)
	{
		if (InputRecord.Piece == EPiece::Own_King)
		{
			KingPos[Hand] = InputRecord.MovePos;
		}
	}
	else
	{
		if (InputRecord.Piece == EPiece::Enemy_King)
		{
			KingPos[Hand] = InputRecord.MovePos;
		}
	}
	POSITION NonePos = { -1, -1 };
	// 持ち駒を置いたのでないなら
	if (InputRecord.SelectPos != NonePos)
	{
		ShogiBoard[InputRecord.SelectPos.y][InputRecord.SelectPos.x] = EPiece::None;
	}

	// 成駒のチェック
	bool IsCallPromFunc = false;
	bool IsProm = false;
	if (!Hand)
	{
		// 持ち駒でなく、相手の陣地に入っている場合関数を呼ぶ
		if (InputRecord.SelectPos != NonePos &&
			InputRecord.MovePos.y <= ENEMY_AREA)
		{
			SelectPromPiece(ShogiBoard, InputRecord.MovePos, &IsProm, Hand);
			IsCallPromFunc = true;
		}
	}
	else
	{
		// 持ち駒でなく、相手の陣地に入っている場合関数を呼ぶ
		if (InputRecord.SelectPos != NonePos && 
			InputRecord.MovePos.y >= OWN_AREA)
		{
			SelectPromPiece(ShogiBoard, InputRecord.MovePos, &IsProm, Hand);
			IsCallPromFunc = true;
		}
	}
	InputRecord.IsCallPromFunc = IsCallPromFunc;
	InputRecord.IsProm = IsProm;

	bool IsCheck = false;
	if (IsCheckMate(ShogiBoard, KingPos[!Hand], !Hand))
	{
		IsCheck = true;
	}

	// 列は右から左で1～9となるので、表示用に値を変換
	// 持ち駒を置いたのでないなら
	if (InputRecord.SelectPos != NonePos)
	{
		// 選択座標を表示用に更新
		HoriConv(&++InputRecord.SelectPos.x);
		++InputRecord.SelectPos.y;
	}
	// 移動座標を表示用に更新
	HoriConv(&++InputRecord.MovePos.x);
	++InputRecord.MovePos.y;
	// 棋譜描画
	RecordsDraw(InputRecord, Hand, IsBack);

	int32_t OutputCheck;
	printf("\n棋譜情報ファイルを作成しますか？(0:する、1:しない):");
	scanf_s("%d", &OutputCheck);
	rewind(stdin);
	if (OutputCheck == 0)
	{
		WriteOutputRecord("RecordData\\GameRecord.txt", SaveRecord, HandNum);
	}

	// 盤描画
	Draw(ShogiBoard);

	// 持ち駒描画
	CapPieceDraw(CapturedPieceNum);

	if (IsCheck)
	{
		CheckMateDraw(Hand);
	}

	// 先手後手交代
	HandNum++;
	Hand = HandNum % 2;
	NowHand = Hand;

	// 持ち駒関数を呼んだ情報をリセット
	IsUseCapFunc = false;
}

bool BackCheck(bool Hand)
{
	if (HandNum == 0)
	{
		return false;
	}

	int32_t Back;
	if (!Hand)
	{
		printf("後手:「待った」をしますか？(0:する 1:しない):");
	}
	else
	{
		printf("先手:「待った」をしますか？(0:する 1:しない):");
	}
	scanf_s("%d", &Back);
	rewind(stdin);

	if (Back == 0)
	{
		IsBack = true;
	}
	else
	{
		IsBack = false;
	}

	return IsBack;
}

// 持ち駒となるかの確認
bool CapPieceConfirm(POSITION MovePos, bool Hand)
{
	int32_t PieceNum = ShogiBoard[MovePos.y][MovePos.x];
	// 駒がある場合
	if (PieceNum != EPiece::None)
	{
		// 王だったら取れないのでfalse
		if (PieceNum == EPiece::Own_King || PieceNum == EPiece::Enemy_King)
		{
			printf("王は取れない\n");
			return false;
		}

		// 先手
		if (!Hand)
		{
			// 動いた先にある駒番号が自分の番号だったらfalse
			if (PieceNum <= EPiece::Own_PromRook)
			{
				printf("自分の駒があります\n");
				return false;
			}
		}
		// 後手
		else
		{
			// 動いた先にある駒番号が自分の番号だったらfalse
			if (PieceNum >= EPiece::Enemy_Gold)
			{
				printf("自分の駒があります\n");
				return false;
			}
		}

		// 駒番号を持ち駒番号に変更する
		int32_t CapPieceNumber = EPiece::None;
		switch (PieceNum)
		{
		case EPiece::Own_Gold:
		case EPiece::Enemy_Gold:
			CapPieceNumber = ECapPiece::Gold;
		case EPiece::Own_Silver:
		case EPiece::Enemy_Silver:
		case EPiece::Own_PromSilver:
		case EPiece::Enemy_PromSilver:
			CapPieceNumber = ECapPiece::Silver;
			break;
		case EPiece::Own_Knight:
		case EPiece::Enemy_Knight:
		case EPiece::Own_PromKnight:
		case EPiece::Enemy_PromKnight:
			CapPieceNumber = ECapPiece::Knight;
			break;
		case EPiece::Own_Lance:
		case EPiece::Enemy_Lance:
		case EPiece::Own_PromLance:
		case EPiece::Enemy_PromLance:
			CapPieceNumber = ECapPiece::Lance;
			break;
		case EPiece::Own_Bishop:
		case EPiece::Enemy_Bishop:
		case EPiece::Own_PromBishop:
		case EPiece::Enemy_PromBishop:
			CapPieceNumber = ECapPiece::Bishop;
			break;
		case EPiece::Own_Rook:
		case EPiece::Enemy_Rook:
		case EPiece::Own_PromRook:
		case EPiece::Enemy_PromRook:
			CapPieceNumber = ECapPiece::Rook;
			break;
		case EPiece::Own_Pawn:
		case EPiece::Enemy_Pawn:
		case EPiece::Own_PromPawn:
		case EPiece::Enemy_PromPawn:
			CapPieceNumber = ECapPiece::Pawn;
			break;
		default:
			break;
		}

		// 駒番号により対象の持ち駒数を増やす
		CapturedPieceNum[Hand][CapPieceNumber]++;

		// 待った用に駒情報を保存
		BackSavePiece = CapPieceNumber;
	}

	return true;
}

bool IsUseCapPiece(int32_t CapPieceNum[EHand::MaxHand][ECapPiece::MaxCap], bool Hand)
{
	// 持ち駒があるかどうか
	bool IsCapPiece[ECapPiece::MaxCap] = { false, };
	int32_t CapPieceNone = 0;
	// Noneは見る必要なし
	for (int32_t i = ECapPiece::Gold; i < ECapPiece::MaxCap; i++)
	{
		if (CapPieceNum[Hand][i] > 0)
		{
			IsCapPiece[i] = true;
		}
		CapPieceNone += IsCapPiece[i];
	}

	// 持ち駒がない
	if (CapPieceNone == 0)
	{
		return false;
	}

	// 持ち駒表示用文字列
	std::string CanUsePiece = "";
	// Noneは見る必要なし
	for (int32_t i = ECapPiece::Gold; i < ECapPiece::MaxCap; i++)
	{
		if (!IsCapPiece[i])
			continue;

		switch (i)
		{
		case ECapPiece::Gold:
			CanUsePiece += "金(1) ";
			break;
		case ECapPiece::Silver:
			CanUsePiece += "銀(2) ";
			break;
		case ECapPiece::Knight:
			CanUsePiece += "桂(3) ";
			break;
		case ECapPiece::Lance:
			CanUsePiece += "香(4) ";
			break;
		case ECapPiece::Bishop:
			CanUsePiece += "角(5) ";
			break;
		case ECapPiece::Rook:
			CanUsePiece += "飛(6) ";
			break;
		case ECapPiece::Pawn:
			CanUsePiece += "歩(7) ";
			break;
		default:
			break;
		}
	}
	printf("使える駒は:%s\n", CanUsePiece.c_str());

	int32_t UseCheck;
	printf("持ち駒を使いますか？(0:使う、1:使わない):");
	scanf_s("%d", &UseCheck);
	rewind(stdin);
	if (UseCheck == 0)
	{
		do
		{
			printf("使う駒の番号を指定:");
			scanf_s("%d", &UseCheck);
			rewind(stdin);

			if (!IsCapPiece[UseCheck])
			{
				printf("ありません\n");
			}
		} while (!IsCapPiece[UseCheck]);
	}
	else
	{
		IsUseCapFunc = true;
		return false;
	}

	CapPieceNum[Hand][UseCheck]--;
	// 先手か後手かによって情報の入れ方が変わる

	if (!Hand)
	{
		InputRecord.Piece = UseCheck;
	}
	else
	{
		InputRecord.Piece = UseCheck + EPiece::Enemy_Gold - 1;
	}
	// 範囲外とする
	SelectPiecePos[Hand] = { -1, -1 };

	// どこに置くかの選択
	bool IsSuccess = false;
	POSITION InputPos;
	while (!IsSuccess)
	{
		if (InputFunc(&InputPos))
		{
			// 列に関しては、右から左で1～9となるので、配列用に値を変換
			HoriConv(&InputPos.x);

			// 駒が置かれていたらやり直し
			if (ShogiBoard[--InputPos.y][--InputPos.x] != EPiece::None)
			{
				printf("そこは駒が置かれています\n");
				continue;
			}

			// 成功
			IsSuccess = true;
		}
		else
		{
			printf("そこは盤の外です\n");
		}
	}

	// 入力情報をグローバルに保存
	MoveInputPos[Hand] = InputPos;
	return true;
}

// 成り駒となるか
void SelectPromPiece(int32_t ShogiBoard[VERT_NUM][HORI_NUM], POSITION MovePos, bool* IsProm, bool Hand)
{
	int32_t PieceNum = ShogiBoard[MovePos.y][MovePos.x];
	POSITION NonePos = { -1, -1 };
	// 持ち駒の場合、成らない
	if (SelectPiecePos[Hand] == NonePos)
	{
		return;
	}

	// 既に成っている場合、何もしない
	if ((PieceNum >= EPiece::Own_PromSilver && EPiece::Own_PromPawn >= PieceNum) ||
		(PieceNum >= EPiece::Enemy_PromSilver && EPiece::Enemy_PromPawn >= PieceNum))
	{
		return;
	}

	int32_t PromCheck;
	// 入力を促す
	if (!Hand)
	{
		printf("先手:成りますか？(0:成る、1:成らない):");
	}
	else
	{
		printf("後手:成りますか？(0:成る、1:成らない):");
	}
	scanf_s("%d", &PromCheck);
	rewind(stdin);
	if (PromCheck == 0)
	{
		// 成駒に変更
		PieceNum += PROM_DIFF;
		ShogiBoard[MovePos.y][MovePos.x] = PieceNum;
		*IsProm = true;
	}
	else
	{
		// 成らない
		*IsProm = false;
		return;
	}
}

// 王手かどうかの判定
bool IsCheckMate(int32_t ShogiBoard[VERT_NUM][HORI_NUM], POSITION KingPos, bool OppositeHand)
{
	int32_t PieceNum = EPiece::None;
	// 王の上列・左側から見る
	POSITION CheckPos = { KingPos.x - 1, KingPos.y - 1 };
	// 盤外なら
	if (CheckPos.y < 0)
	{
		// 中列を見る
		CheckPos.y = KingPos.y;
	}

	for (int32_t i = CheckPos.y; i <= KingPos.y + 1; i++)
	{
		// 上列・下列
		if (i == KingPos.y - 1 || i == KingPos.y + 1)
		{
			for (int32_t j = CheckPos.x; j <= KingPos.x + 1; j++)
			{
				// 盤外処理
				if (j < 0 || HORI_NUM - 1 < j)
					continue;

				PieceNum = ShogiBoard[i][j];
				// 何もなければ処理を飛ばす
				if (PieceNum == EPiece::None)
					continue;

				// 左・右
				if (j == CheckPos.x || j == KingPos.x + 1)
				{
					// 現在の手によって判定が変わる
					if (!OppositeHand)
					{
						if (PieceNum == EPiece::Enemy_Silver || PieceNum == EPiece::Enemy_Bishop || PieceNum == EPiece::Enemy_King ||
							(PieceNum >= Enemy_PromBishop && Enemy_PromRook >= PieceNum))
						{
							return true;
						}
					}
					else
					{
						if (PieceNum == EPiece::Own_Silver || PieceNum == EPiece::Own_Bishop || PieceNum == EPiece::Own_King ||
							(PieceNum >= Own_PromBishop && Own_PromRook >= PieceNum))
						{
							return true;
						}
					}
				}
				// 真ん中
				else if (j == KingPos.x)
				{
					// 現在の手によって判定が変わる
					if (!OppositeHand)
					{
						if (PieceNum == EPiece::Enemy_Gold || (PieceNum >= EPiece::Enemy_Rook && EPiece::Enemy_King >= PieceNum) ||
							(PieceNum >= Enemy_PromSilver && Enemy_PromPawn >= PieceNum))
						{
							return true;
						}
					}
					else
					{
						if (PieceNum == EPiece::Own_Gold || (PieceNum >= EPiece::Own_Rook && EPiece::Own_King >= PieceNum) ||
							(PieceNum >= Own_PromSilver && Own_PromPawn >= PieceNum))
						{
							return true;
						}
					}
				}
			}
		}
		// 中列
		else if (i == KingPos.y)
		{
			for (int32_t j = CheckPos.x; j <= KingPos.x + 1; j += 2)
			{
				// 盤外処理
				if (j < 0 || HORI_NUM - 1 < j)
					continue;

				PieceNum = ShogiBoard[i][j];
				// 何もなければ処理を飛ばす
				if (PieceNum == EPiece::None)
					continue;

				// 左・右
				// 現在の手によって判定が変わる
				if (!OppositeHand)
				{
					if (PieceNum == EPiece::Enemy_Gold || PieceNum == EPiece::Enemy_Rook || PieceNum == EPiece::Enemy_King ||
						(PieceNum >= Enemy_PromSilver && Enemy_PromPawn >= PieceNum))
					{
						return true;
					}
				}
				else
				{
					if (PieceNum == EPiece::Own_Gold || PieceNum == EPiece::Own_Rook || PieceNum == EPiece::Own_King ||
						(PieceNum >= Own_PromSilver && Own_PromPawn >= PieceNum))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

// 今の手の表示
void NowHandDisp(bool Hand)
{
	if (!Hand)
	{
		printf("先手の番\n");
	}
	else
	{
		printf("後手の番\n");
	}
}

void Draw(int32_t PieceMap[VERT_NUM][HORI_NUM])
{
	// 漢字を入れる
	const int32_t MaxUseKanji = 9;
	std::string str[MaxUseKanji] =
	{
		"一", "二", "三", "四", "五", "六", "七", "八", "九"
	};

	for (int32_t i = 0; i < MaxUseKanji; i++)
	{
		Kanji[i] = str[i];
	}

	std::string HoriStr = "";

	for (int32_t i = 0; i < VERT_NUM; i++)
	{
		HoriStr += Kanji[i];
		for (int32_t j = 0; j < HORI_NUM; j++)
		{
			HoriStr += "│ ";

			if (PieceMap[i][j] == EPiece::None)
			{
				HoriStr += "□│";
				continue;
			}
			else if (PieceMap[i][j] <= EPiece::Own_PromPawn)
			{
				// 赤
				HoriStr += "\x1b[31m";
			}
			else if (PieceMap[i][j] >= EPiece::Enemy_Gold)
			{
				// 水色
				HoriStr += "\x1b[36m";
			}

			switch (PieceMap[i][j])
			{
			case EPiece::Own_Gold:
			case EPiece::Own_PromSilver:
			case EPiece::Own_PromKnight:
			case EPiece::Own_PromLance:
			case EPiece::Enemy_Gold:
			case EPiece::Enemy_PromSilver:
			case EPiece::Enemy_PromKnight:
			case EPiece::Enemy_PromLance:
				HoriStr += "金";
				break;
			case EPiece::Own_Silver:
			case EPiece::Enemy_Silver:
				HoriStr += "銀";
				break;
			case EPiece::Own_Knight:
			case EPiece::Enemy_Knight:
				HoriStr += "桂";
				break;
			case EPiece::Own_Lance:
			case EPiece::Enemy_Lance:
				HoriStr += "香";
				break;
			case EPiece::Own_Bishop:
			case EPiece::Enemy_Bishop:
				HoriStr += "角";
				break;
			case EPiece::Own_Rook:
			case EPiece::Enemy_Rook:
				HoriStr += "飛";
				break;
			case EPiece::Own_Pawn:
			case EPiece::Enemy_Pawn:
				HoriStr += "歩";
				break;
			case EPiece::Own_King:
				HoriStr += "玉";
				break;
			case EPiece::Enemy_King:
				HoriStr += "王";
				break;
			case EPiece::Own_PromPawn:
			case EPiece::Enemy_PromPawn:
				HoriStr += "と";
				break;
			case EPiece::Own_PromBishop:
			case EPiece::Enemy_PromBishop:
				HoriStr += "龍";
				break;
			case EPiece::Own_PromRook:
			case EPiece::Enemy_PromRook:
				HoriStr += "馬";
				break;
			default:
				break;
			}
			// 色戻す
			HoriStr += "\x1b[39m│";
		}
		HoriStr += "\n　└───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘\n";
		if (i != VERT_NUM - 1)
		{
			HoriStr += "　┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐\n";
		}
	}

	printf("　　9　　8　　7　　6　　5　　4　　3　　2　　1\n");
	printf("　┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐\n");
	printf("%s", HoriStr.c_str());
}

// 持ち駒の表示
void CapPieceDraw(int32_t CapPieceNum[EHand::MaxHand][ECapPiece::MaxCap])
{
	for (int32_t i = 0; i < EHand::MaxHand; i++)
	{
		std::string PieceStr = "";
		if (i == EHand::First)
		{
			PieceStr += "先手の持ち駒:";
		}
		else
		{
			PieceStr += "後手の持ち駒:";
		}

		printf("%s", PieceStr.c_str());
		for (int32_t j = ECapPiece::Gold; j < ECapPiece::MaxCap; j++)
		{
			if (CapPieceNum[i][j] == 0)
				continue;

			switch (j)
			{
			case ECapPiece::Gold:
				printf("金×%d ", CapPieceNum[i][j]);
				break;
			case ECapPiece::Silver:
				printf("銀×%d ", CapPieceNum[i][j]);
				break;
			case ECapPiece::Knight:
				printf("桂×%d ", CapPieceNum[i][j]);
				break;
			case ECapPiece::Lance:
				printf("香×%d ", CapPieceNum[i][j]);
				break;
			case ECapPiece::Bishop:
				printf("角×%d ", CapPieceNum[i][j]);
				break;
			case ECapPiece::Rook:
				printf("飛×%d ", CapPieceNum[i][j]);
				break;
			case ECapPiece::Pawn:
				printf("歩×%d ", CapPieceNum[i][j]);
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
}

void RecordsDraw(RECORDINFO Record, bool Hand, bool IsBack)
{
	int32_t temp = HandNum;
	// 配列外処理
	if (HandNum >= MAX_SAVE)
	{
		printf("それ以上保存できません\n");

		// 上書きする形で
		temp = MAX_SAVE - 1;
	}

	// 待った からの呼び出しでなければ
	if (!IsBack)
	{
		SaveRecord[temp++] = Record;
	}

	for (int32_t i = 0; i < temp; i++)
	{
		std::string PieceCate = "";

		switch (SaveRecord[i].Piece)
		{
		case EPiece::Own_Gold:
		case EPiece::Enemy_Gold:
			PieceCate += "金";
			break;
		case EPiece::Own_Silver:
		case EPiece::Enemy_Silver:
			PieceCate += "銀";
			break;
		case EPiece::Own_PromSilver:
		case EPiece::Enemy_PromSilver:
			PieceCate += "成銀";
			break;
		case EPiece::Own_Knight:
		case EPiece::Enemy_Knight:
			PieceCate += "桂";
			break;
		case EPiece::Own_PromKnight:
		case EPiece::Enemy_PromKnight:
			PieceCate += "成桂";
			break;
		case EPiece::Own_Lance:
		case EPiece::Enemy_Lance:
			PieceCate += "香";
			break;
		case EPiece::Own_PromLance:
		case EPiece::Enemy_PromLance:
			PieceCate += "成香";
			break;
		case EPiece::Own_Bishop:
		case EPiece::Enemy_Bishop:
			PieceCate += "角";
			break;
		case EPiece::Own_PromBishop:
		case EPiece::Enemy_PromBishop:
			PieceCate += "龍";
			break;
		case EPiece::Own_Rook:
		case EPiece::Enemy_Rook:
			PieceCate += "飛";
			break;
		case EPiece::Own_PromRook:
		case EPiece::Enemy_PromRook:
			PieceCate += "馬";
			break;
		case EPiece::Own_Pawn:
		case EPiece::Enemy_Pawn:
			PieceCate += "歩";
			break;
		case EPiece::Own_PromPawn:
		case EPiece::Enemy_PromPawn:
			PieceCate += "と";
			break;
		case EPiece::Own_King:
			PieceCate += "玉";
			break;
		case EPiece::Enemy_King:
			PieceCate += "王";
			break;
		default:
			break;
		}

		std::string HandRecord = "";
		if (!SaveRecord[i].Hand)
		{
			HandRecord += "▲";
		}
		else
		{
			HandRecord += "▽";
		}

		// iが0以降、前のPosと一致していたら表記変更
		if (i > 0 && SaveRecord[i].MovePos == SaveRecord[i - 1].MovePos)
		{
			printf("%s同%s", HandRecord.c_str(), PieceCate.c_str());
		}
		else
		{
			printf("%s%d%s%s", HandRecord.c_str(), SaveRecord[i].MovePos.x, Kanji[SaveRecord[i].MovePos.y - 1].c_str(), PieceCate.c_str());
		}

		if (SaveRecord[i].IsCallPromFunc)
		{
			if (SaveRecord[i].IsProm)
			{
				printf("成 ");
			}
			else
			{
				printf("不成 ");
			}
		}
		else
		{
			printf(" ");
		}

		// 改行条件
		if ((i + 1) % 10 == 0)
		{
			printf("\n");
		}
	}
}

// 王手かの表示
void CheckMateDraw(bool Hand)
{
	printf("\n");
	if (!Hand)
	{
		printf("先手:王手です\n");
	}
	else
	{
		printf("後手:王手です\n");
	}
}

// 棋譜データの出力
void WriteOutputRecord(const char* pFileName, RECORDINFO Record[MAX_SAVE], int32_t HandNum)
{
	FILE* fp;
	errno_t ErrCheck;

	// ファイルの作成
	ErrCheck = fopen_s(&fp, pFileName, "wt");

	// エラー処理
	if (ErrCheck != 0 || fp == NULL)
	{
		printf("ファイルを作成できませんでした。\n");
		exit(ErrCheck);
	}
	else
	{
		printf("ファイルを作成しました。\n\n");
	}

	// ファイルに書き込み
	int32_t temp;
	temp = HandNum + 1;
	for (int32_t i = 0; i < temp; i++)
	{
		// 先手か後手か
		fprintf_s(fp, "%s,", Record[i].Hand ? "true" : "false");
		// 駒移動座標
		fprintf_s(fp, "%d,%d,", Record[i].SelectPos.x, Record[i].SelectPos.y);
		// 駒移動座標
		fprintf_s(fp, "%d,%d,", Record[i].MovePos.x, Record[i].MovePos.y);
		// 駒種類
		fprintf_s(fp, "%d,", Record[i].Piece);
		// 成駒関数を呼んだかどうか
		fprintf_s(fp, "%s,", Record[i].IsCallPromFunc ? "true" : "false");
		// 成ったかどうか
		fprintf_s(fp, "%s,", Record[i].IsProm ? "true" : "false");
		// 次のデータ入力の為改行
		fprintf(fp, "\n");
	}

	// ファイルを閉じる
	fclose(fp);

	// 少し待つ
	Sleep(500);
}