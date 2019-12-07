#pragma once
#ifndef __SHOGI_H__
#define __SHOGI_H__

#include <iostream>
#include <string>
#include <Windows.h>

#define VERT_NUM		9		// 縦の数
#define HORI_NUM		9		// 横の数
#define MAX_SAVE		100		// 最大保存数

enum EPiece			// 駒の種類
{
	None,
	Gold,
	Silver,
	Knight,
	Lance,
	Bishop,
	Rook,
	Pawn,
	King,
	PromSilver,
	PromKnight,
	PromLance,
	PromPawn,
	PromBishop,
	PromRook,
	MaxPiece
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

	POSITION() { x = 0, y = 0; };

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
	bool Hand;		// false:先手 true:後手
	POSITION Pos;	// 座標
	int32_t Piece;	// 駒の種類
	
	struct RECORDINFO()
	{
		Hand = false;
		Pos = { 0, 0 };
		Piece = EPiece::None;
	}
};

std::string Kanji[9];											// 漢字格納文字列
int32_t PieceMap[VERT_NUM][HORI_NUM];							// 将棋盤
int32_t WhichHand;												// 合計手数
bool NowHand;													// 今が先手か後手か
POSITION TopPiecePos[EHand::MaxHand][HORI_NUM];					// 先手後手それぞれの一番上の駒座標
int32_t NowTopHoriNum[EHand::MaxHand];							// 自身の駒が同じ行に何個あるか
POSITION SelectPiecePos[EHand::MaxHand];						// 「待った」用駒選択座標
POSITION MoveInputPos[EHand::MaxHand];							// 「待った」用移動座標
int32_t BackSavePiece[EHand::MaxHand];							// 待った用、その手で持ち駒を獲得してたら相手に戻す
int32_t CapturedPieceNum[EHand::MaxHand][EPiece::King];			// 持ち駒格納変数
RECORDINFO InputRecord[EHand::MaxHand];							// 入力された記録
RECORDINFO SaveRecord[MAX_SAVE];								// 入力した記録を保存する

void Initialize();												// 初期化
bool InputFunc(POSITION* pPos);									// 入力関数
void InputPos_IsPiece(int32_t PieceMap[VERT_NUM][HORI_NUM],
					POSITION* pInputPos, bool Hand);			// 駒があるかの判定
bool InputPos_IsMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM],
					POSITION* pInputPos, bool Hand);			// 駒が動けるかの判定
void Update(bool Hand);											// 更新処理
bool BackCheck(bool Hand);										// 「待った」の判定
bool CapPieceConfirm(POSITION InputPos, bool Hand);				// 持ち駒となるかの確認
bool IsUseCapPiece(int32_t CapPieceNum[EHand::MaxHand][EPiece::King],
										bool Hand);				// 持ち駒を使うのか
bool IsSelectPromPiece(bool Hand);								// 成り駒関連
bool IsCheck(bool Hand);										// 王手/詰みかどうか
void NowHandDisp(bool Hand);									// 今の手の表示
void Draw(int32_t PieceMap[VERT_NUM][HORI_NUM], bool Hand);		// 将棋盤描画
void CapPieceDraw(
	int32_t CapPieceNum[EHand::MaxHand][EPiece::King]);			// 持ち駒の表示
void RecordsDraw(bool Hand);									// 保存した記録の表示

#endif /* __SHOGI_H__ */

// メモ 罫線素片
// ─ │
// ┌ ┐
// ┘ └
// ├ ┬
// ┤ ┴ 
// ┼
