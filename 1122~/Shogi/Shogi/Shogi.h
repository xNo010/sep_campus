#pragma once
#ifndef __SHOGI_H__
#define __SHOGI_H__

#include <iostream>
#include <string>
#include <Windows.h>

#define VERT_NUM		9
#define HORI_NUM		9
#define MAX_SAVE		100

enum EPiece
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

enum EHand
{
	First,
	Second,
	MaxHand
};

struct POSITION
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
	int32_t Which;	// 0:êÊéË 1:å„éË
	POSITION Pos;
	int32_t Piece;
	
	struct RECORDINFO()
	{
		Which = false;
		Pos = { 0, 0 };
		Piece = EPiece::None;
	}
};

std::string Kanji[9];
int32_t PieceMap[VERT_NUM][HORI_NUM];
int32_t WhichHand;
POSITION TopPiecePos[EHand::MaxHand][HORI_NUM];
int32_t NowTopHoriNum;
POSITION SaveInputPos;
RECORDINFO InputRecord;
RECORDINFO SaveRecord[MAX_SAVE];
int32_t SaveCount;

void Initialize();
void InputFunc(POSITION* pPos);
bool InputPos_IfPiece(int32_t PieceMap[VERT_NUM][HORI_NUM]);
bool InputPos_IfMovePiece(int32_t PieceMap[VERT_NUM][HORI_NUM]);
void Update();
void RecordDraw();
void Draw();

#endif /* __SHOGI_H__ */

// ÉÅÉÇ årê¸ëfï–
// Ñü Ñ†
// Ñ° Ñ¢
// Ñ£ Ñ§
// Ñ• Ñ¶
// Ñß Ñ® 
// Ñ©
