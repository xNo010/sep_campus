#pragma once
#ifndef __SHOGI_H__
#define __SHOGI_H__

#include <iostream>
#include <string>
#include <Windows.h>

#define MAP_HEIGHT		17
#define MAP_WIDTH		18
#define VERT_NUM		9
#define HORI_NUM		9
#define PIECE_ALLOCNUM	(20 * 2)
#define MAX_SAVE		100

enum EChipCate
{
	Piece_King,
	Piece_None,
	Piece_GoldGeneral,
	Piece_SilverGeneral,
	Piece_Knight,
	Piece_Lance,
	Piece_Rook,
	Piece_Bishop,
	Piece_Pawn,
	Space,
	Cross,
};

enum EPieceCate
{
	None,
	King,
	GoldGeneral,
	SilverGeneral,
	Knight,
	Lance,
	Rook,
	Bishop,
	Pawn,
	Max
};

enum EHand
{
	First,
	Second
};

struct POSITION
{
	int32_t x;
	int32_t y;

	void operator = (const POSITION& p)
	{
		x = p.x;
		y = p.y;
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
struct PIECEINFO
{
	bool Which;	// true:êÊéË false:å„éË
	POSITION Pos;
	int32_t Piece;
};

std::string Kanji[9];
int32_t PieceMap[VERT_NUM][HORI_NUM];
int32_t DrawMap[MAP_HEIGHT][MAP_WIDTH];
PIECEINFO Shogi[PIECE_ALLOCNUM];
PIECEINFO ShogiHistory[MAX_SAVE];

void Initialize();
void Draw();

#endif /* __SHOGI_H__ */

// ÉÅÉÇ årê¸ëfï–
// Ñ™ Ñ´
// Ñ¨ Ñ≠
// ÑÆ ÑØ
// Ñ∞ Ñ±
// Ñ≤ Ñ≥
// Ñ¥
// Ñü Ñ†
// Ñ° Ñ¢
// Ñ£ Ñ§
// Ñ• Ñ¶
// Ñß Ñ® 
// Ñ©
