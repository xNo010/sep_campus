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
	King,
	Gold,
	Silver,
	Knight,
	Lance,
	Bishop,
	Rook,
	Pawn,
	Max
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
	int32_t Which;	// 0:先手 1:後手
	POSITION Pos;	// 座標
	int32_t Piece;	// 駒の種類
	
	struct RECORDINFO()
	{
		Which = false;
		Pos = { 0, 0 };
		Piece = EPiece::None;
	}
};

std::string Kanji[9];							// 漢字格納
int32_t PieceMap[VERT_NUM][HORI_NUM];			// 将棋盤
int32_t WhichHand;								// 合計手数
int32_t NowHand;								// 今が先手か後手かの判断
POSITION TopPiecePos[EHand::MaxHand][HORI_NUM];	// 先手後手それぞれの一番上の駒座標
int32_t NowTopHoriNum[EHand::MaxHand];			// 判別の際、駒が同じ行に何個あるか
POSITION SaveInputPos[EHand::MaxHand];			// 「待った」用駒選択座標
POSITION MoveInputPos[EHand::MaxHand];			// 「待った」用移動座標
RECORDINFO InputRecord;							// 入力された記録
RECORDINFO SaveRecord[MAX_SAVE];				// 入力した記録を保存する

void Initialize();													// 初期化
bool InputFunc(POSITION* pPos);										// 入力関数
bool InputPos_IfPiece(int32_t PieceMap[VERT_NUM][HORI_NUM]);		// 駒があるかの判定
bool InputPos_IfMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM]);	// 駒が動けるかの判定
void Update();														// 更新処理
bool BackCheck();													// 「待った」の判定
void Draw();														// マップ描画
void RecordsDraw();													// 保存した記録の表示

#endif /* __SHOGI_H__ */

// メモ 罫線素片
// ─ │
// ┌ ┐
// ┘ └
// ├ ┬
// ┤ ┴ 
// ┼
