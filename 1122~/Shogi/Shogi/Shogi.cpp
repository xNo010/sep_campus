#include "Shogi.h"

int main()
{
	Initialize();

	while (1)
	{
		if (!InputPos_IfPiece(PieceMap, &SaveInputPos[NowHand]))
		{
			printf("\n");
			continue;
		}
		if (!InputPos_IfMovePiece(PieceMap, &MoveInputPos[NowHand]))
		{
			printf("\n");
			continue;
		}

		Update();
	}
}

void Initialize()
{
	const int32_t MaxUseKanji = 9;
	std::string str[MaxUseKanji] =
	{
		"一", "二", "三", "四", "五", "六", "七", "八", "九"
	};

	for (int32_t i = 0; i < MaxUseKanji; i++)
	{
		Kanji[i] = str[i];
	}

	WhichHand = 0;
	NowHand = WhichHand % 2;
	ZeroMemory(TopPiecePos, sizeof(TopPiecePos));
	ZeroMemory(NowTopHoriNum, sizeof(NowTopHoriNum));
	ZeroMemory(SaveInputPos, sizeof(SaveInputPos));
	ZeroMemory(MoveInputPos, sizeof(MoveInputPos));
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

				TopPiecePos[EHand::First][j].y = VERT_NUM - i - 1;
				TopPiecePos[EHand::Second][j].y = i;
			}
			break;
		default:
			break;
		}
	}

	// 最初の表示
	Draw();
}

bool InputFunc(POSITION* pPos)
{
	ZeroMemory(pPos, sizeof(POSITION));
	printf("列の選択(整数で入力):");
	scanf_s("%d", &pPos->x);
	// 将棋盤の外に行ってしまったかのチェック
	if (pPos->x <= 0 || HORI_NUM < pPos->x)
	{
		printf("そこは盤の外です\n");
		return false;
	}
	printf("段の選択(整数で入力):");
	scanf_s("%d", &pPos->y);
	if (pPos->y <= 0 || VERT_NUM < pPos->y)
	{
		printf("そこは盤の外です\n");
		return false;
	}

	return true;
}

bool InputPos_IfPiece(int32_t PieceMap[VERT_NUM][HORI_NUM], POSITION* pInputPos)
{
	if (NowHand == EHand::First)
	{
		printf("先手の番\n");
	}
	else
	{
		printf("後手の番\n");
	}

	printf("動かす駒の選択\n");
	if (!InputFunc(&SaveInputPos[NowHand]))
	{
		return false;
	}

	// 駒があるかのチェック
	if (PieceMap[--SaveInputPos[NowHand].y][--SaveInputPos[NowHand].x] == EPiece::None)
	{
		printf("そこに駒はありません\n");
		return false;
	}

	// 駒はあったが、自分の駒なのか
	// 先手後手それぞれで、現在どの位置に駒があるかのチェック
	if (NowHand == EHand::First)
	{
		if (SaveInputPos[NowHand].y < TopPiecePos[NowHand][NowTopHoriNum[NowHand]].y)
		{
			printf("自身の駒ではありません\n");
			return false;
		}
	}
	else
	{
		if (SaveInputPos[NowHand].y > TopPiecePos[NowHand][NowTopHoriNum[NowHand]].y)
		{
			printf("自身の駒ではありません\n");
			return false;
		}
	}

	// 先手後手が最低一回回った後かつ、入力したY座標が保存された一番上の座標と一致したら
	if (WhichHand > EHand::Second && SaveInputPos[NowHand].y == TopPiecePos[NowHand][NowTopHoriNum[NowHand]].y)
	{
		// X座標から同じ駒を探しだす
		for (int32_t i = 0; i <= NowTopHoriNum[NowHand]; i++)
		{
			// 同じ座標のものを見つけたら終わり
			if (SaveInputPos[NowHand].x == TopPiecePos[NowHand][i].x)
				break;

			if (i == NowTopHoriNum[NowHand])
			{
				printf("自身の駒ではありません\n");
				return false;
			}
		}
	}

	// 駒情報を入れる
	InputRecord.Piece = PieceMap[SaveInputPos[NowHand].y][SaveInputPos[NowHand].x];

	return true;
}

bool InputPos_IfMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM], POSITION* pInputPos)
{
	printf("駒を動かす座標選択\n");
	if (!InputFunc(&MoveInputPos[NowHand]))
	{
		return false;
	}

	// 計算用に更新
	MoveInputPos[NowHand] = { MoveInputPos[NowHand].x - 1, MoveInputPos[NowHand].y - 1 };

	// 同じマスを選んでいたらfalse
	if (SaveInputPos[NowHand] == MoveInputPos[NowHand])
	{
		printf("同じマスは選べません\n");
		return false;
	}

	// 駒座標と移動座標の差分を取得
	POSITION DiffCell = SaveInputPos[NowHand] - MoveInputPos[NowHand];

	// 2マス以上進もうとした際、一度に2マス以上進めない駒かのチェック
	if ((abs(DiffCell.x) > 1 || abs(DiffCell.y) > 1)&& 
		((InputRecord.Piece >= EPiece::King && EPiece::Silver >= InputRecord.Piece) || InputRecord.Piece == EPiece::Pawn))
	{
		printf("選択した駒はそこに移動できません\n");
		return false;
	}

	// 置けるが、そこに駒があるかのチェック
	if (PieceMap[MoveInputPos[NowHand].y][MoveInputPos[NowHand].x] != EPiece::None)
	{
		printf("すでに駒が置かれています\n");
		return false;
	}

	// 選択駒が移動可能な座標かチェック
	const int32_t OneCell = 1;
	const int32_t TwoCell = 2;
	switch (InputRecord.Piece)
	{
	case EPiece::King:
		break;
	case EPiece::Gold:
		if (NowHand == EHand::First)
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
		if (NowHand == EHand::First)
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
		if (NowHand == EHand::First)
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
		if (NowHand == EHand::First)
		{
			if (abs(DiffCell.x) != 0 || DiffCell.y < 0)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (abs(DiffCell.x) != 0 || DiffCell.y > 0)
			{
				printf("選択した駒はそこに移動できません\n");
				return false;
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
	case EPiece::Rook:
		if (DiffCell.x > 0 && DiffCell.y != 0 ||
			DiffCell.y > 0 && DiffCell.x != 0)
		{
			printf("選択した駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Pawn:
		if (NowHand == EHand::First)
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

void Update()
{
	// 最初に「待った」をするかのチェック
	if (BackCheck())
	{
		// 一手戻す
		WhichHand--;
		NowHand = WhichHand % 2;
		PieceMap[SaveInputPos[NowHand].y][SaveInputPos[NowHand].x] = PieceMap[MoveInputPos[NowHand].y][MoveInputPos[NowHand].x];
		PieceMap[MoveInputPos[NowHand].y][MoveInputPos[NowHand].x] = EPiece::None;

		// 盤描画
		Draw();

		// 棋譜用に一つ戻す
		WhichHand--;
		// 棋譜描画
		RecordsDraw();
		// 元に戻す
		WhichHand++;

		// 待ったの処理をしたので終了
		return;
	}
	else
	{
		// 現在の手がどちらか知る
		NowHand = WhichHand % 2;
	}

	// 先手/後手の駒それぞれで一番先にあるのはどこなのかチェック
	if (NowHand == EHand::First)
	{
		// 入力されたY座標が上にある駒の座標より小さければTop更新-先手側は値が小さいほど上に行く
		if (MoveInputPos[NowHand].y < TopPiecePos[NowHand][NowTopHoriNum[NowHand]].y)
		{
			// 全ての座標を更新する
			TopPiecePos[NowHand][NowTopHoriNum[NowHand]] = { MoveInputPos[NowHand].x, MoveInputPos[NowHand].y };
		}
	}
	else
	{
		// 入力されたY座標が上にある駒の座標より大きければTop更新-後手側は値が大きいほど上に行く
		if (MoveInputPos[NowHand].y > TopPiecePos[NowHand][NowTopHoriNum[NowHand]].y)
		{
			// 全ての座標を更新する
			TopPiecePos[NowHand][NowTopHoriNum[NowHand]] = { MoveInputPos[NowHand].x, MoveInputPos[NowHand].y };
		}
	}

	// トップが同じ状態で複数あるならx座標で見る
	if (MoveInputPos[NowHand].y == TopPiecePos[NowHand][NowTopHoriNum[NowHand]].y &&
		MoveInputPos[NowHand].x != TopPiecePos[NowHand][NowTopHoriNum[NowHand]].x)
	{
		TopPiecePos[NowHand][++NowTopHoriNum[NowHand]] = { MoveInputPos[NowHand].x, MoveInputPos[NowHand].y };
	}

	// 先手か後手か + 移動座標を入れる
	InputRecord.Which = NowHand;
	InputRecord.Pos = MoveInputPos[NowHand];

	// 入力値を参考に値の入れ替え
	PieceMap[MoveInputPos[NowHand].y][MoveInputPos[NowHand].x] = InputRecord.Piece;
	PieceMap[SaveInputPos[NowHand].y][SaveInputPos[NowHand].x] = EPiece::None;

	// 盤描画
	Draw();

	// 棋譜描画
	RecordsDraw();

	// 先手後手交代
	WhichHand++;
	NowHand = WhichHand % 2;
}

bool BackCheck()
{
	if (WhichHand == 0)
		return false;

	int32_t Back;
	printf("「待った」をしますか(0:する 1:しない):\n");
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

void Draw()
{
	std::string HoriStr = "";

	for (int32_t i = 0; i < VERT_NUM; i++)
	{
		HoriStr += Kanji[i];
		for (int32_t j = 0; j < HORI_NUM; j++)
		{
			switch (PieceMap[i][j])
			{
			case EPiece::None:
				HoriStr += "│ □│";
				break;
			case EPiece::King:
				HoriStr += "│ 王│";
				break;
			case EPiece::Gold:
				HoriStr += "│ 金│";
				break;
			case EPiece::Silver:
				HoriStr += "│ 銀│";
				break;
			case EPiece::Knight:
				HoriStr += "│ 桂│";
				break;
			case EPiece::Lance:
				HoriStr += "│ 香│";
				break;
			case EPiece::Rook:
				HoriStr += "│ 飛│";
				break;
			case EPiece::Bishop:
				HoriStr += "│ 角│";
				break;
			case EPiece::Pawn:
				HoriStr += "│ 歩│";
				break;
			default:
				break;
			}
		}
		HoriStr += "\n　└───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘└───┘\n";
		if (i != VERT_NUM - 1)
		{
			HoriStr += "　┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐\n";
		}
	}

	//printf("\033[2J");
	printf("　　1　　2　　3　　4　　5　　6　　7　　8　　9\n");
	printf("　┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐┌───┐\n");
	printf("%s", HoriStr.c_str());
}

void RecordsDraw()
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

	SaveRecord[temp] = InputRecord;

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
			Record += "先-" + Kanji[SaveRecord[i].Pos.y];
			printf("%s%d-%s\n", Record.c_str(), SaveRecord[i].Pos.x + 1, PieceCate.c_str());
		}
		else
		{
			Record += "後-" + Kanji[SaveRecord[i].Pos.y];
			printf("%s%d-%s\n", Record.c_str(), SaveRecord[i].Pos.x + 1, PieceCate.c_str());
		}
	}
	printf("\n");
}