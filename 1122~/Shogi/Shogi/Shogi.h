#pragma once
#ifndef __SHOGI_H__
#define __SHOGI_H__

#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>

#define VERT_NUM		9					// 縦の数
#define HORI_NUM		9					// 横の数
#define OWN_AREA		(VERT_NUM - 3)		// 自陣
#define ENEMY_AREA		(VERT_NUM / 3 - 1)	// 敵陣
#define PROM_DIFF		7					// 成る際の駒番号差分
#define MAX_SAVE		100					// 最大保存数

enum EPiece			// 駒の種類
{
	None,
	Own_Gold,
	Own_Silver,
	Own_Knight,
	Own_Lance,
	Own_Bishop,
	Own_Rook,
	Own_Pawn,
	Own_King,
	Own_PromSilver,
	Own_PromKnight,
	Own_PromLance,
	Own_PromBishop,
	Own_PromRook,
	Own_PromPawn,
	Enemy_Gold,
	Enemy_Silver,
	Enemy_Knight,
	Enemy_Lance,
	Enemy_Bishop,
	Enemy_Rook,
	Enemy_Pawn,
	Enemy_King,
	Enemy_PromSilver,
	Enemy_PromKnight,
	Enemy_PromLance,
	Enemy_PromBishop,
	Enemy_PromRook,
	Enemy_PromPawn,
	MaxPiece
};

enum ECapPiece
{
	NoneCap,
	Gold,
	Silver,
	Knight,
	Lance,
	Bishop,
	Rook,
	Pawn,
	MaxCap
};

enum EHand			// 先手後手
{
	First,
	Second,
	MaxHand
};

struct POSITION		// 座標
{
	int32_t x;
	int32_t y;

	POSITION() { x = 0, y = 0; }

	POSITION(int32_t ax, int32_t ay)
	{
		x = ax;
		y = ay;
	}

	void operator = (const POSITION& p)
	{
		x = p.x;
		y = p.y;
	}

	POSITION operator - (const POSITION& p) const
	{
		return POSITION(x - p.x, y - p.y);
	}

	int32_t operator == (const POSITION& p) const
	{
		return x == p.x && y == p.y;
	}
	int32_t operator != (const POSITION& p) const
	{
		return x != p.x || y != p.y;
	}

};

struct RECORDINFO
{
	bool Hand;				// false:先手 true:後手
	POSITION SelectPos;		// 駒選択座標
	POSITION MovePos;		// 駒移動座標
	int32_t Piece;			// 駒の種類
	bool IsCallPromFunc;	// 成駒関数を呼んだかどうかの有無
	bool IsProm;			// 成ったかどうかの有無

	struct RECORDINFO()
	{
		Hand = false;
		SelectPos = { -1, -1 };
		MovePos = { -1, -1 };
		Piece = EPiece::None;
		IsCallPromFunc = IsProm = false;
	}
};

std::string Kanji[VERT_NUM];									// 漢字格納文字列
int32_t ShogiBoard[VERT_NUM][HORI_NUM];							// 将棋盤
int32_t HandNum;												// 合計手数
bool NowHand;													// 今が先手か後手か
bool IsBack;													// 待ったをしたかどうか
bool IsUseCapFunc;												// 持ち駒使用関数を呼んだかどうか
POSITION SelectPiecePos[EHand::MaxHand];						// 待った用に先手後手分の駒選択座標
POSITION MoveInputPos[EHand::MaxHand];							// 待った用に先手後手分の移動座標
POSITION KingPos[EHand::MaxHand];								// 王の座標
int32_t BackSavePiece;											// 待った用、その手で持ち駒を獲得してたら相手に戻す
int32_t CapturedPieceNum[EHand::MaxHand][ECapPiece::MaxCap];	// 持ち駒格納変数
RECORDINFO InputRecord;											// 入力された記録
RECORDINFO SaveRecord[MAX_SAVE];								// 入力した記録を保存する

void Initialize();												// 初期化
void HoriConv(int32_t* HoriPos);								// 列数値の配列用⇔表示用変換
bool InputFunc(POSITION* pPos);									// 入力関数
bool InputPos_IsPiece(int32_t ShogiBoard[VERT_NUM][HORI_NUM],
							bool Hand);							// 駒があるかの判定
bool InputPos_IsMovePiece(int32_t ShogiBoard[VERT_NUM][HORI_NUM],
							bool Hand);							// 駒が動けるかの判定
void Update(bool Hand);											// 更新処理
bool BackCheck(bool Hand);										// 「待った」の判定
bool CapPieceConfirm(POSITION MovePos, bool Hand);				// 持ち駒となるかの確認
bool IsUseCapPiece(int32_t CapPieceNum[EHand::MaxHand][ECapPiece::MaxCap],
	bool Hand);													// 持ち駒を使うのか
void SelectPromPiece(int32_t ShogiBoard[VERT_NUM][HORI_NUM],
	POSITION MovePos, bool* IsProm, bool Hand);					// 成り駒になるかの選択
bool IsCheckMate(int32_t ShogiBoard[VERT_NUM][HORI_NUM], 
	POSITION KingPos, bool OppositeHand);						// 王手/詰みかどうか
void NowHandDisp(bool Hand);									// 今の手の表示
void Draw(int32_t ShogiBoard[VERT_NUM][HORI_NUM]);				// 将棋盤描画
void CapPieceDraw(
	int32_t CapPieceNum[EHand::MaxHand][ECapPiece::MaxCap]);	// 持ち駒の表示
void RecordsDraw(bool Hand, bool IsBack);						// 保存した記録の表示
void CheckMateDraw(bool Hand);									// 王手かの表示
void WriteOutputRecord(const char* pFileName, RECORDINFO Output[MAX_SAVE],
	int32_t HandNum);											// 棋譜データの出力

#endif /* __SHOGI_H__ */

// メモ 罫線素片
// ─ │
// ┌ ┐
// ┘ └
// ├ ┬
// ┤ ┴ 
// ┼
