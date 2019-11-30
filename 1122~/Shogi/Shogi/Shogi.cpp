#include "Shogi.h"

int main()
{
	Initialize();

	while (1)
	{
		if (!InputPos_IfPiece(PieceMap))
		{
			printf("\n");
			continue;
		}
		if (!InputPos_IfMovePiece(PieceMap))
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
	ZeroMemory(TopPiecePos, sizeof(TopPiecePos));
	NowTopHoriNum = 0;
	SaveInputPos = { 0, 0 };
	SaveCount = 0;
	ZeroMemory(SaveRecord, sizeof(SaveRecord));
	ZeroMemory(PieceMap, sizeof(PieceMap));

	// 駒をマップに配置
	const int32_t Center = 4;
	int32_t PieceInfo = EPiece::King;
	for (int32_t i = 0; i < VERT_NUM / 3; i++)
	{
		int32_t LeftShift = Center, RightShift = Center;
		switch (i)
		{
		case 0:
			PieceMap[i][Center] = PieceInfo;
			PieceMap[VERT_NUM - i - 1][Center] = PieceInfo;
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
			PieceInfo++;
			LeftShift -= 2;
			PieceMap[i][LeftShift] = PieceInfo;
			PieceMap[VERT_NUM - i - 1][LeftShift] = PieceInfo;
			
			PieceInfo++;
			RightShift += 2;
			PieceMap[i][RightShift] = PieceInfo;
			PieceMap[VERT_NUM - i - 1][RightShift] = PieceInfo;
			break;
		case 2:
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

	Draw();
}

void InputFunc(POSITION* pPos)
{
	ZeroMemory(pPos, sizeof(POSITION));
	printf("数字部分を数値で入力:");
	scanf_s("%d", &pPos->x);
	printf("漢数字部分を数値で入力:");
	scanf_s("%d", &pPos->y);
}

bool InputPos_IfPiece(int32_t PieceMap[VERT_NUM][HORI_NUM])
{
	printf("動かす駒の選択\n");
	InputFunc(&SaveInputPos);

	// 将棋盤の外に行ってしまったかのチェック
	if (SaveInputPos.x <= 0 || HORI_NUM < SaveInputPos.x ||
		SaveInputPos.y <= 0 || VERT_NUM < SaveInputPos.y)
	{
		printf("そこは盤の外です\n");
		return false;
	}

	// 駒があるかのチェック
	if (PieceMap[--SaveInputPos.y][--SaveInputPos.x] == EPiece::None)
	{
		printf("そこに駒はありません\n");
		return false;
	}

	int32_t NowHand = WhichHand % 2;
	// 駒はあったが、自分の駒なのか
	// 先手後手それぞれで、現在どの位置に駒があるかのチェック
	if (NowHand == EHand::First)
	{
		if (SaveInputPos.y < TopPiecePos[NowHand][NowTopHoriNum].y)
		{
			printf("自身の駒ではありません\n");
			return false;
		}
		
		if (TopPiecePos[NowHand][NowTopHoriNum].y <= TopPiecePos[NowHand][NowTopHoriNum].y)
		{
			for (int32_t i = 0; i < NowTopHoriNum; i++)
			{
				// 同じ座標のものを見つけたら終わり
				if (SaveInputPos.x == TopPiecePos[NowHand][i].x)
					break;

				if (i == NowTopHoriNum - 1)
				{
					printf("自身の駒ではありません\n");
					return false;
				}
			}
		}
	}
	else
	{
		if (SaveInputPos.y > TopPiecePos[NowHand][NowTopHoriNum].y)
		{
			printf("自身の駒ではありません\n");
			return false;
		}

		if (TopPiecePos[NowHand][NowTopHoriNum].y >= TopPiecePos[NowHand][NowTopHoriNum].y)
		{
			for (int32_t i = 0; i < NowTopHoriNum; i++)
			{
				// 同じ座標のものを見つけたら終わり
				if (SaveInputPos.x == TopPiecePos[NowHand][NowTopHoriNum].x)
					break;

				if (i == NowTopHoriNum - 1)
				{
					printf("自身の駒ではありません\n");
					return false;
				}
			}
		}
	}

	// 成功したので先手か後手か+駒情報を入れる
	InputRecord.Which = NowHand;
	InputRecord.Piece = PieceMap[SaveInputPos.y][SaveInputPos.x];

	return true;
}

bool InputPos_IfMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM])
{
	printf("駒を動かす座標選択\n");
	InputFunc(&InputRecord.Pos);

	InputRecord.Pos = { InputRecord.Pos.x - 1, InputRecord.Pos.y - 1 };

	// 同じマスを選んでいたらfalse
	if (SaveInputPos == InputRecord.Pos)
	{
		printf("同じマスは選べません\n");
		return false;
	}
	POSITION diff = SaveInputPos - InputRecord.Pos;
	// 2マス以上進もうとした際、一度に2マス以上進めない駒かのチェック
	if ((abs(diff.x) > 1 || abs(diff.y) > 1)&& 
		((InputRecord.Piece >= EPiece::King && EPiece::Silver >= InputRecord.Piece) || InputRecord.Piece == EPiece::Pawn))
	{
		printf("選択した駒は、そこには置けません\n");
		return false;
	}

	// 置けるが、そこに駒があるかのチェック
	if (PieceMap[InputRecord.Pos.y][InputRecord.Pos.x] != EPiece::None)
	{
		printf("すでに駒が置かれています\n");
		return false;
	}

	// 先手後手かを知る
	int32_t NowHand = WhichHand % 2;
	// 選択駒が移動可能な座標かチェック
	POSITION DiffCell = SaveInputPos - InputRecord.Pos;
	const int32_t OneCell = 1;
	const int32_t TwoCell = 2;
	switch (InputRecord.Piece)
	{
	case EPiece::King:
		break;
	case EPiece::Gold:
		if (NowHand == 0)
		{
			if (DiffCell.y == +OneCell && (abs(DiffCell.x) == OneCell))
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (DiffCell.y == -OneCell && (abs(DiffCell.x) == OneCell))
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	case EPiece::Silver:
		if (NowHand == 0)
		{
			if ((DiffCell.x == 0 && DiffCell.y == -OneCell) || (abs(DiffCell.x) == OneCell && DiffCell.y == 0))
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if ((DiffCell.x == 0 && DiffCell.y == +OneCell) || (abs(DiffCell.x) == OneCell && DiffCell.y == 0))
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	case EPiece::Knight:
		if (NowHand == 0)
		{
			if (abs(DiffCell.x) == 0 || DiffCell.y != TwoCell)
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (abs(DiffCell.x) == 0 || DiffCell.y != -TwoCell)
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	case EPiece::Lance:
		if (NowHand == 0)
		{
			if (abs(DiffCell.x) != 0 || DiffCell.y < 0)
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (abs(DiffCell.x) != 0 || DiffCell.y > 0)
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		break;
	case EPiece::Bishop:
		if (DiffCell.x + DiffCell.y != 0)
		{
			printf("この駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Rook:
		if (DiffCell.x > 0 && DiffCell.y != 0 ||
			DiffCell.y > 0 && DiffCell.x != 0)
		{
			printf("この駒はそこに移動できません\n");
			return false;
		}
		break;
	case EPiece::Pawn:
		if (NowHand == 0)
		{
			if (DiffCell.x != 0 || DiffCell.y != +OneCell)
			{
				printf("この駒はそこに移動できません\n");
				return false;
			}
		}
		else
		{
			if (DiffCell.x != 0 || DiffCell.y != -OneCell)
			{
				printf("この駒はそこに移動できません\n");
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
	// 入力値を参考に値の入れ替え
	int32_t temp = PieceMap[InputRecord.Pos.y][InputRecord.Pos.x];
	PieceMap[InputRecord.Pos.y][InputRecord.Pos.x] = PieceMap[SaveInputPos.y][SaveInputPos.x];
	PieceMap[SaveInputPos.y][SaveInputPos.x] = temp;

	int32_t NowHand = WhichHand % 2;
	// 先手/後手の駒それぞれで一番先にあるのはどこなのかチェック
	if (NowHand == 0)
	{
		// 入力された値が先にある駒より小さければTop更新
		if (InputRecord.Pos.y < TopPiecePos[NowHand][NowTopHoriNum].y)
		{
			TopPiecePos[NowHand][NowTopHoriNum] = { InputRecord.Pos.x, InputRecord.Pos.y };
		}

		// トップが同じ状態で複数あるならx座標で見る
		if (InputRecord.Pos.y == TopPiecePos[NowHand][NowTopHoriNum].y &&
			InputRecord.Pos.x != TopPiecePos[NowHand][NowTopHoriNum].x)
		{
			TopPiecePos[NowHand][++NowTopHoriNum] = { InputRecord.Pos.x, InputRecord.Pos.y };
		}
	}
	else
	{
		// 入力された値が先にある駒より大きければTop更新
		if (InputRecord.Pos.y > TopPiecePos[NowHand][NowTopHoriNum].y)
		{
			TopPiecePos[NowHand][NowTopHoriNum] = { InputRecord.Pos.x, InputRecord.Pos.y };
		}

		// トップが同じ状態で複数あるならx座標で見る
		if (InputRecord.Pos.y == TopPiecePos[NowHand][NowTopHoriNum].y &&
			InputRecord.Pos.x != TopPiecePos[NowHand][NowTopHoriNum].x)
		{
			TopPiecePos[NowHand][++NowTopHoriNum] = { InputRecord.Pos.x, InputRecord.Pos.y };
		}
	}

	// 盤描画
	Draw();

	// 棋譜描画
	RecordDraw();
}

void RecordDraw()
{
	// 配列外処理
	if (WhichHand == MAX_SAVE - 1)
	{
		printf("それ以上保存できません\n");
		return;
	}

	SaveRecord[WhichHand] = InputRecord;

	int32_t NowHand = WhichHand % 2;
	std::string PieceCate = "";
	switch (SaveRecord[WhichHand].Piece)
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

	// 
	for (int32_t i = 0; i <= WhichHand; i++)
	{
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

	// 攻守交替
	WhichHand++;
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