#pragma once
#ifndef __SHOGI_H__
#define __SHOGI_H__

#include <iostream>
#include <string>
#include <Windows.h>

#define MAP_HEIGHT	17
#define MAP_WIDTH	18
#define VERT_NUM	9
#define HORI_NUM	9

enum EChipCate
{
	None,
	Space,
	Piece,
	Cross
};

enum EPieceCate
{
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

struct SHOGIINFO
{

};

std::string Kanji[9];
int32_t PieceMap[VERT_NUM][HORI_NUM];
int32_t DrawMap[MAP_HEIGHT][MAP_WIDTH];

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
// ÑßÑ® 
// Ñ©
