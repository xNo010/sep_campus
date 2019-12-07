#include "Shogi.h"

int main()
{
	Initialize();

	// 最初の表示
	Draw(PieceMap, NowHand);

	while (1)
	{
		NowHandDisp(NowHand);

		// 持ち駒の使用有無
		if (!IsUseCapPiece(CapturedPieceNum, NowHand))
		{
			// 駒がある座標を指定
			InputPos_IsPiece(PieceMap, SelectPiecePos, NowHand);

			// 駒を動かす座標を指定
			if (!InputPos_IsMovePiece(PieceMap, MoveInputPos, NowHand))
			{
				printf("\n");
				continue;
			}
		}

		Update(NowHand);
	}
}

void Initialize()
{
	WhichHand = 0;
	NowHand = WhichHand % 2;
	ZeroMemory(TopPiecePos, sizeof(TopPiecePos));
	ZeroMemory(NowTopHoriNum, sizeof(NowTopHoriNum));
	ZeroMemory(SelectPiecePos, sizeof(SelectPiecePos));
	ZeroMemory(MoveInputPos, sizeof(MoveInputPos));
	ZeroMemory(BackSavePiece, sizeof(BackSavePiece));
	ZeroMemory(CapturedPieceNum, sizeof(CapturedPieceNum));
	ZeroMemory(SaveRecord, sizeof(SaveRecord));
	ZeroMemory(PieceMap, sizeof(PieceMap));

	// 駒をマップに配置
	const int32_t Center = 4;
	const int32_t ThreeCell = 3;
	int32_t PieceInfo = EPiece::King;
	for (int32_t i = 0; i < VERT_NUM / 3; i++)
	{
		int32_t LeftShift = Center, RightShift = Center;
		switch (i)
		{
		case 0:
			// 玉/王
			PieceMap[i][Center] = PieceInfo;
			PieceMap[VERT_NUM - i - 1][Center] = PieceInfo;

			PieceInfo = EPiece::None;
			// 金～香
			while (LeftShift != 0 && RightShift != 0)
			{
				PieceInfo++;
				PieceMap[i][--LeftShift] = PieceInfo;
				PieceMap[i][++RightShift] = PieceInfo;
				PieceMap[VERT_NUM - i - 1][LeftShift] = PieceInfo;
				PieceMap[VERT_NUM - i - 1][RightShift] = PieceInfo;
			}
			break;
		case 1:
			// 角
			PieceInfo++;
			LeftShift -= ThreeCell;
			RightShift += ThreeCell;

			PieceMap[i][RightShift] = PieceInfo;
			PieceMap[VERT_NUM - i - 1][LeftShift] = PieceInfo;
			
			// 飛
			PieceInfo++;
			PieceMap[i][LeftShift] = PieceInfo;
			PieceMap[VERT_NUM - i - 1][RightShift] = PieceInfo;
			break;
		case 2:
			// 歩
			PieceInfo++;
			for (int32_t j = 0; j < HORI_NUM; j++)
			{
				PieceMap[i][j] = PieceInfo;
				PieceMap[VERT_NUM - i - 1][j] = PieceInfo;

				// それぞれの一番上のY座標をセット
				TopPiecePos[EHand::First][j] = { j, VERT_NUM - i - 1 };
				TopPiecePos[EHand::Second][j] = { j, i };
			}
			NowTopHoriNum[EHand::First] = HORI_NUM - 1;
			NowTopHoriNum[EHand::Second] = HORI_NUM - 1;
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

// 駒があるかどうかのチェック
void InputPos_IsPiece(int32_t PieceMap[VERT_NUM][HORI_NUM], POSITION* pInputPos, bool Hand)
{
	printf("・動かす駒の選択\n");
	// 入力に失敗したらやり直し
	if (!InputFunc(&SelectPiecePos[Hand]))
	{
		printf("そこは盤の外です\n");
		InputPos_IsPiece(PieceMap, &SelectPiecePos[Hand], Hand);
	}

	// 駒がなければやり直し
	if (PieceMap[--SelectPiecePos[Hand].y][--SelectPiecePos[Hand].x] == EPiece::None)
	{
		printf("そこに駒はありません\n");
		InputPos_IsPiece(PieceMap, &SelectPiecePos[Hand], Hand);
	}

	// 駒はあるが、自分の駒でなければやり直し
	if (!Hand)
	{
		if (SelectPiecePos[Hand].y < TopPiecePos[Hand][0].y)
		{
			printf("自身の駒ではありません\n");
			InputPos_IsPiece(PieceMap, &SelectPiecePos[Hand], Hand);
		}
	}
	else
	{
		if (SelectPiecePos[Hand].y > TopPiecePos[Hand][0].y)
		{
			printf("自身の駒ではありません\n");
			InputPos_IsPiece(PieceMap, &SelectPiecePos[Hand], Hand);
		}
	}

	// 入力したY座標が保存されている一番上の座標と一致したら
	if (SelectPiecePos[Hand].y == TopPiecePos[Hand][0].y)
	{
		// X座標から同じ駒を探す
		for (int32_t i = 0; i <= NowTopHoriNum[Hand]; i++)
		{
			// 同じ座標のものを見つけたらループを抜ける
			if (SelectPiecePos[Hand].x == TopPiecePos[Hand][i].x)
				break;

			// 見つけられなかったらやり直し
			if (i == NowTopHoriNum[Hand])
			{
				printf("自身の駒ではありません\n");
				// もう一度呼ぶ
				InputPos_IsPiece(PieceMap, &SelectPiecePos[Hand], Hand);
			}
		}
	}

	// 駒情報を入れる
	InputRecord[Hand].Piece = PieceMap[SelectPiecePos[Hand].y][SelectPiecePos[Hand].x];
}

bool InputPos_IsMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM], POSITION* pInputPos, bool Hand)
{
	printf("・駒を動かす座標選択\n");
	// 入力に失敗したらやり直し
	if (!InputFunc(&MoveInputPos[Hand]))
	{
		printf("そこは盤の外です\n");
		return false;
	}

	// 計算用に更新
	MoveInputPos[Hand].x--, MoveInputPos[Hand].y--;

	// 同じマスを選んでいたらやり直し
	if (SelectPiecePos[Hand] == MoveInputPos[Hand])
	{
		printf("同じマスは選べません\n");
		return false;
	}

	// 駒座標と移動座標が何マス分離れているか
	POSITION DiffCell = SelectPiecePos[Hand] - MoveInputPos[Hand];

	// 2マス以上進もうとした際、一度に2マス以上進めない駒だったらここでやり直し
	if ((abs(DiffCell.x) > 1 || abs(DiffCell.y) > 1)&& 
		((InputRecord[Hand].Piece <= EPiece::Silver) || (InputRecord[Hand].Piece >= EPiece::Pawn) && InputRecord[Hand].Piece <= EPiece::PromPawn))
	{
		printf("選択した駒は移動できません\n");
		return false;
	}

	// 持ち駒にならないならやり直し
	if (!CapPieceConfirm(MoveInputPos[Hand], Hand))
	{
		return false;
	}

	// 選択駒が移動可能な座標かチェック
	const int32_t OneCell = 1;
	const int32_t TwoCell = 2;
	// 駒によってfalse条件が変わる
	switch (InputRecord[Hand].Piece)
	{
	case EPiece::Gold:
	case EPiece::PromSilver:
	case EPiece::PromKnight:
	case EPiece::PromLance:
	case EPiece::PromPawn:
		if (!Hand)
		{
			if (DiffCell.y == +OneCell && (abs(DiffCell.x) == OneCell))
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (DiffCell.y == -OneCell && (abs(DiffCell.x) == OneCell))
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	case EPiece::Silver:
		if (!Hand)
		{
			if ((DiffCell.x == 0 && DiffCell.y == -OneCell) || (abs(DiffCell.x) == OneCell && DiffCell.y == 0))
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if ((DiffCell.x == 0 && DiffCell.y == +OneCell) || (abs(DiffCell.x) == OneCell && DiffCell.y == 0))
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	case EPiece::Knight:
		if (!Hand)
		{
			if (abs(DiffCell.x) == 0 || DiffCell.y != TwoCell)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (abs(DiffCell.x) == 0 || DiffCell.y != -TwoCell)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	case EPiece::Lance:
		if (!Hand)
		{
			if (abs(DiffCell.x) != 0 || DiffCell.y < 0)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
			// 次に縦列に駒があるかの検索
			for (int32_t i = SelectPiecePos[Hand].y; i > MoveInputPos[Hand].y; i--)
			{
				// 駒があったらそこまで動けない
				if (PieceMap[i][MoveInputPos[Hand].x] != EPiece::None)
				{
					printf("駒を飛び越すことはできません\n");
					return false;
				}
			}
		}
		else
		{
			if (abs(DiffCell.x) != 0 || DiffCell.y > 0)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
			// 次に縦列に駒があるかの検索
			for (int32_t i = SelectPiecePos[Hand].y; i < MoveInputPos[Hand].y; i++)
			{
				// 駒があったらそこまで動けない
				if (PieceMap[i][MoveInputPos[Hand].x] != EPiece::None)
				{
					printf("駒を飛び越すことはできません\n");
					return false;
				}
			}
		}
		break;
	case EPiece::Bishop:
		if (DiffCell.x + DiffCell.y != 0)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::PromBishop:
		if (DiffCell.x + DiffCell.y != 0 ||
			DiffCell.x != 0 && abs(DiffCell.y) == 1 ||
			DiffCell.y != 0 && abs(DiffCell.x) == 1)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Rook:
		if (DiffCell.x > 0 && DiffCell.y != 0 ||
			DiffCell.y > 0 && DiffCell.x != 0)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Pawn:
		if (!Hand)
		{
			if (DiffCell.x != 0 || DiffCell.y != +OneCell)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (DiffCell.x != 0 || DiffCell.y != -OneCell)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	default:
		break;
	}

	return true;
}

void Update(bool Hand)
{
	// 最初に「待った」をするかのチェック
	if (BackCheck(Hand))
	{
		// 一手戻す
		WhichHand--;
		Hand = WhichHand % 2;
		// 元いた位置に戻す
		PieceMap[SelectPiecePos[Hand].y][SelectPiecePos[Hand].x] = PieceMap[MoveInputPos[Hand].y][MoveInputPos[Hand].x];
		// 元あった情報にする
		PieceMap[MoveInputPos[Hand].y][MoveInputPos[Hand].x] = BackSavePiece[Hand];
		// 持ち駒から消す
		CapturedPieceNum[Hand][BackSavePiece[Hand]]--;

		// 盤描画
		Draw(PieceMap, Hand);

		// 棋譜描画用に一つ戻す
		WhichHand--;
		// どちらの手か再計算
		Hand = WhichHand % 2;
		// 棋譜描画
		RecordsDraw(Hand);
		
		// 元に戻す
		WhichHand++;
		// どちらの手か再計算
		Hand = WhichHand % 2;
		NowHand = Hand;
		// 待ったの処理をしたので終了
		return;
	}

	// 先手/後手の駒それぞれで一番先にあるのはどこなのかチェック
	if (!Hand)
	{
		// 入力されたY座標が上にあった駒の座標より小さければ、Top更新-先手側は値が小さいほど上に行く
		if (MoveInputPos[Hand].y < TopPiecePos[Hand][0].y)
		{
			// トップ座標を更新する
			TopPiecePos[Hand][0] = MoveInputPos[Hand];
			// トップで並んでいる数をリセット
			NowTopHoriNum[Hand] = 0;
		}
	}
	else
	{
		// 入力されたY座標が上にあった駒の座標より大きければ、Top更新-後手側は値が大きいほど上に行く
		if (MoveInputPos[Hand].y > TopPiecePos[Hand][0].y)
		{
			// トップ座標を更新する
			TopPiecePos[Hand][0] = MoveInputPos[Hand];
			// トップで並んでいる数をリセット
			NowTopHoriNum[Hand] = 0;
		}
	}

	// トップが同じ状態で複数あるなら、x座標で管理
	if (TopPiecePos[Hand][0].y == MoveInputPos[Hand].y &&
		TopPiecePos[Hand][0].x != MoveInputPos[Hand].x)
	{
		// 座標を更新する
		TopPiecePos[Hand][++NowTopHoriNum[Hand]] = MoveInputPos[Hand];
	}

	// 先手か後手かの情報 + 移動座標を入れる
	InputRecord[Hand].Hand = Hand;
	InputRecord[Hand].Pos = MoveInputPos[Hand];

	// 入力値を参考に値の入れ替え
	PieceMap[MoveInputPos[Hand].y][MoveInputPos[Hand].x] = InputRecord[Hand].Piece;
	POSITION NonePos = { 0, 0 };
	// 持ち駒からの置きでないなら
	if (SelectPiecePos[Hand] != NonePos)
	{
		PieceMap[SelectPiecePos[Hand].y][SelectPiecePos[Hand].x] = EPiece::None;
	}

	// 盤描画
	Draw(PieceMap, Hand);

	// 持ち駒描画
	CapPieceDraw(CapturedPieceNum);

	// 棋譜描画
	RecordsDraw(Hand);

	// 先手後手交代
	WhichHand++;
	Hand = WhichHand % 2;
	NowHand = Hand;
}

bool BackCheck(bool Hand)
{
	if (WhichHand == 0)
		return false;

	int32_t Back;
	if (!Hand)
	{
		printf("後手:「待った」をしますか(0:する 1:しない):\n");
	}
	else
	{
		printf("先手:「待った」をしますか(0:する 1:しない):\n");
	}
	scanf_s("%d", &Back);

	if (Back == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

// 持ち駒となるかの確認
bool CapPieceConfirm(POSITION InputPos, bool Hand)
{
	int32_t PieceMapNum = PieceMap[MoveInputPos[Hand].y][MoveInputPos[Hand].x];
	// 駒がある場合
	if (PieceMapNum != EPiece::None && PieceMapNum != EPiece::King)
	{
		// 今の手と逆の手
		bool OppositeHand = !Hand;

		// 先手
		if (!Hand)
		{
			// 相手のトップY座標より大きかったら自分の駒
			if (MoveInputPos[Hand].y > TopPiecePos[OppositeHand][0].y)
			{
				printf("自分の駒が置かれています\n");
				return false;
			}
		}
		// 後手
		else
		{
			// 相手のトップY座標より小さかったら自分の駒
			if (MoveInputPos[Hand].y < TopPiecePos[OppositeHand][0].y)
			{
				printf("自分の駒が置かれています\n");
				return false;
			}
		}

		// 自分の駒じゃないかX座標で見る
		if (MoveInputPos[Hand].y == TopPiecePos[Hand][0].y)
		{
			for (int32_t i = 0; i <= NowTopHoriNum[Hand]; i++)
			{
				// X座標で見ると自分の駒だった
				if (MoveInputPos[Hand] == TopPiecePos[Hand][i])
				{
					printf("自分の駒が置かれています\n");
					return false;
				}
			}
		}
		
		// falseじゃなければ持ち駒にする処理(玉/王は除く)
		CapturedPieceNum[Hand][PieceMapNum]++;
		// 相手のトップにあったものであった場合、取られたので情報を消す
		// 相手のトップと自分のトップが一緒になり判別できなくなる。
		// 自分のトップにはあとで更新で入る。
		if (MoveInputPos[Hand].y == TopPiecePos[OppositeHand][0].y)
		{
			if (NowTopHoriNum[OppositeHand] == 0)
			{
				TopPiecePos[OppositeHand][0] = TopPiecePos[OppositeHand][NowTopHoriNum[OppositeHand] + 1];
			}
			else if (NowTopHoriNum[OppositeHand] != HORI_NUM - 1)
			{
				TopPiecePos[OppositeHand][0] = TopPiecePos[OppositeHand][NowTopHoriNum[OppositeHand]-- + 1];
			}
			else
			{
				// 横一列全てどちらかだけの駒ということ
				// X座標が一致する番号を最後の情報と交換する
				for (int32_t i = 0; i <= NowTopHoriNum[OppositeHand]; i++)
				{
					if (MoveInputPos[Hand].x == TopPiecePos[OppositeHand][i].x)
					{
						TopPiecePos[OppositeHand][i] = TopPiecePos[OppositeHand][NowTopHoriNum[OppositeHand]--];
						break;
					}
				}
			}
		}
		// 待った用に駒情報を保存
		BackSavePiece[Hand] = PieceMapNum;
	}
	else if (PieceMapNum == EPiece::King)
	{
		// 王は王手の判定が別にあるが一時ここに
		printf("王は取れない\n");
		return false;
	}
	// 駒がない場合
	else
	{
		// 取った駒はない
		BackSavePiece[Hand] = EPiece::None;
		return true;
	}

	return true;
}

bool IsUseCapPiece(int32_t CapPieceNum[EHand::MaxHand][EPiece::King], bool Hand)
{
	// 持ち駒があるかどうか
	bool IsCapPiece[EPiece::MaxPiece - 1] = { false, };
	int32_t CapPieceNone = 0;
	// NoneとKingは見る必要なし
	for (int32_t i = EPiece::Gold; i < EPiece::King; i++)
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
	// NoneとKingは見る必要なし
	for (int32_t i = EPiece::Gold; i < EPiece::King; i++)
	{
		if (!IsCapPiece[i])
			continue;

		switch (i)
		{
		case EPiece::Gold:
			CanUsePiece += "金(1) ";
			break;
		case EPiece::Silver:
			CanUsePiece += "銀(2) ";
			break;
		case EPiece::Knight:
			CanUsePiece += "桂(3) ";
			break;
		case EPiece::Lance:
			CanUsePiece += "香(4) ";
			break;
		case EPiece::Bishop:
			CanUsePiece += "角(5) ";
			break;
		case EPiece::Rook:
			CanUsePiece += "飛(6) ";
			break;
		case EPiece::Pawn:
			CanUsePiece += "歩(7) ";
			break;
		default:
			break;
		}
	}
	printf("使える駒は:%s\n", CanUsePiece.c_str());

	int32_t IsUse;
	printf("持ち駒を使いますか？(0:使う、1:使わない):");
	scanf_s("%d", &IsUse);
	if (IsUse == 0)
	{
		do
		{
			printf("使う駒の番号を指定:");
			scanf_s("%d", &IsUse);

			if (!IsCapPiece[IsUse])
			{
				printf("ありません\n");
			}
		} while (!IsCapPiece[IsUse]);
	}
	else
	{
		return false;
	}

	CapPieceNum[Hand][IsUse]--;
	InputRecord[Hand].Piece = IsUse;
	// 範囲外とする
	SelectPiecePos[Hand] = { 0, 0 };

	// どこに置くかの選択
	bool IsSuccess = false;
	while (!IsSuccess)
	{
		if (InputFunc(&MoveInputPos[Hand]))
		{
			// 駒が置かれていたらやり直し
			if (PieceMap[--MoveInputPos[Hand].y][--MoveInputPos[Hand].x] != EPiece::None)
			{
				printf("そこは駒が置かれています\n");
				continue;
			}

			// 成功
			IsSuccess = true;
		}
	}

	return true;
}

bool IsCheck(bool Hand)
{
	// 入力された駒情報から、近くに王があるか判定
	switch (InputRecord[Hand].Piece)
	{
	case EPiece::Gold:
	case EPiece::PromSilver:
	case EPiece::PromKnight:
	case EPiece::PromLance:
	case EPiece::PromPawn:
		if (!Hand)
		{
		}
		else
		{
		}
		break;
	case EPiece::Silver:
		if (!Hand)
		{
		}
		else
		{
		}
		break;
	case EPiece::Knight:
		if (!Hand)
		{
		}
		else
		{
		}
		break;
	case EPiece::Lance:
		if (!Hand)
		{
		}
		else
		{
		}
		break;
	case EPiece::Bishop:
		break;
	case EPiece::PromBishop:
		break;
	case EPiece::Rook:
		break;
	case EPiece::Pawn:
		break;
	default:
		break;
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

void Draw(int32_t PieceMap[VERT_NUM][HORI_NUM], bool Hand)
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
				HoriStr += "□";
			}

			if (TopPiecePos[EHand::First][NowTopHoriNum[EHand::First]].y <= i)
			{
				// 相手とトップが一緒だったら相手の駒じゃないか見る
				if (TopPiecePos[EHand::First][NowTopHoriNum[EHand::First]].y == TopPiecePos[EHand::Second][NowTopHoriNum[EHand::Second]].y)
				{
					for (int32_t k = 0; k <= NowTopHoriNum[!Hand]; k++)
					{
						if (TopPiecePos[EHand::Second][k].x == j)
						{
							HoriStr += "\x1b[36m";
							break;
						}

						if (k == NowTopHoriNum[!Hand])
						{
							HoriStr += "\x1b[31m";
						}
					}
				}
				else
				{
					HoriStr += "\x1b[31m";
				}
			}
			else if (TopPiecePos[EHand::Second][NowTopHoriNum[EHand::Second]].y >= i &&
				TopPiecePos[EHand::First][NowTopHoriNum[EHand::First]].y != i)
			{
				HoriStr += "\x1b[36m";
			}
			switch (PieceMap[i][j])
			{
			case EPiece::Gold:
				HoriStr += "金";
				break;
			case EPiece::Silver:
				HoriStr += "銀";
				break;
			case EPiece::Knight:
				HoriStr += "桂";
				break;
			case EPiece::Lance:
				HoriStr += "香";
				break;
			case EPiece::Rook:
				HoriStr += "飛";
				break;
			case EPiece::Bishop:
				HoriStr += "角";
				break;
			case EPiece::Pawn:
				HoriStr += "歩";
				break;
			case EPiece::King:
				HoriStr += "王";
				break;
			case EPiece::PromSilver:
				HoriStr += "王";
				break;
			case EPiece::PromKnight:
				HoriStr += "王";
				break;
			case EPiece::PromLance:
				HoriStr += "王";
				break;
			case EPiece::PromPawn:
				HoriStr += "王";
				break;
			case EPiece::PromBishop:
				HoriStr += "龍";
				break;
			case EPiece::PromRook:
				HoriStr += "馬";
				break;
			default:
				break;
			}
			HoriStr += "\x1b[39m";
			HoriStr += "│";
		}
		HoriStr += "\n　└───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘\n";
		if (i != VERT_NUM - 1)
		{
			HoriStr += "　┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐\n";
		}
	}

	printf("　　1　　2　　3　　4　　5　　6　　7　　8　　9\n");
	printf("　┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐\n");
	printf("%s", HoriStr.c_str());
}

// 持ち駒の表示
void CapPieceDraw(int32_t CapPieceNum[EHand::MaxHand][EPiece::King])
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
		for (int32_t j = EPiece::Gold; j < EPiece::King; j++)
		{
			if (CapPieceNum[i][j] == 0)
				continue;

			switch (j)
			{
			case EPiece::Gold:
				printf("金×%d ", CapPieceNum[i][j]);
				break;
			case EPiece::Silver:
				printf("銀×%d ", CapPieceNum[i][j]);
				break;
			case EPiece::Knight:
				printf("桂×%d ", CapPieceNum[i][j]);
				break;
			case EPiece::Lance:
				printf("香×%d ", CapPieceNum[i][j]);
				break;
			case EPiece::Rook:
				printf("飛×%d ", CapPieceNum[i][j]);
				break;
			case EPiece::Bishop:
				printf("角×%d ", CapPieceNum[i][j]);
				break;
			case EPiece::Pawn:
				printf("歩×%d ", CapPieceNum[i][j]);
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
}

void RecordsDraw(bool Hand)
{
	int32_t temp = WhichHand;
	// 配列外処理
	if (WhichHand == MAX_SAVE)
	{
		printf("それ以上保存できません\n");

		// 上書きする形で
		temp = WhichHand - 1;
		return;
	}

	SaveRecord[temp] = InputRecord[Hand];

	for (int32_t i = 0; i <= temp; i++)
	{
		std::string PieceCate = "";

		switch (SaveRecord[i].Piece)
		{
		case EPiece::King:
			PieceCate += "王";
			break;
		case EPiece::Gold:
			PieceCate += "金";
			break;
		case EPiece::Silver:
			PieceCate += "銀";
			break;
		case EPiece::Knight:
			PieceCate += "桂";
			break;
		case EPiece::Lance:
			PieceCate += "香";
			break;
		case EPiece::Rook:
			PieceCate += "飛";
			break;
		case EPiece::Bishop:
			PieceCate += "角";
			break;
		case EPiece::Pawn:
			PieceCate += "歩";
			break;
		default:
			break;
		}

		std::string Record = "";
		if (i % 2 == 0)
		{
			Record += "▲";
		}
		else
		{
			Record += "▽";
		}
		printf("%s%d%s-%s ", Record.c_str(), SaveRecord[i].Pos.x + 1, Kanji[SaveRecord[i].Pos.y].c_str(), PieceCate.c_str());
		if ((i + 1) % 2 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}