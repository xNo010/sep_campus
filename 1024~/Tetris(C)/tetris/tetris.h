#pragma once
#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm.lib")

// MoveBlock[2][2]Ç™íÜêSÅAÇªÇ±Ç…çáÇÌÇπÇƒblockÇÃå`Çê∂ê¨

#define BLOCK_NUM		5
#define MAP_WIDTH		10
#define MAP_HEIGHT		25
#define MAX_BLOCK_TABLE 30

enum EChip
{
	None,
	Exist,
	OutSide
};

enum EBlockCategory
{
	Square,
	Stick,
	Convex,
	Z,
	InvZ,
	L,
	InvL,
	MaxNum
};

enum ERotationType
{
	Standard,
	Right,
	Inverse,
	Left
};

enum EOffsetCoord
{
	MoreMoreMin = -2,
	MoreMin,
	Min,
	Max = 5,
	MoreMax,
	MoreMoreMax,
};

bool Initialize();
void EntryBlock(short Data[BLOCK_NUM][BLOCK_NUM]);
void Update(COORD* Position, short RotateInfo, bool* End);
void RotationCheck(COORD* BlockPosition, int32_t RotateInfo);
void Draw();

short Map[MAP_HEIGHT][MAP_WIDTH];
short Zangai[MAP_HEIGHT][MAP_WIDTH];
short SaveZangai[MAP_HEIGHT][MAP_WIDTH];

COORD FirstPosition;
COORD NewPosition;
short RotateDir;
short SaveRotateInfo;
const short MovingValue = 1;
short SaveBlock[BLOCK_NUM][BLOCK_NUM];
short MoveBlock[BLOCK_NUM][BLOCK_NUM];
short EntryBlockNum;

short EntryBlockTable[MAX_BLOCK_TABLE];

bool LeftRotateFlag;
bool LeftMoveFlag;
bool RightMoveFlag;
bool PushDownFlag;
bool PushReturnFlag;
bool EndFlag;

BYTE Key[MAX_PATH];

#endif /* __TETRIS_H__ */

