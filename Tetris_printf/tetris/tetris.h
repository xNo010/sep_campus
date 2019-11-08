#pragma once
#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm.lib")

// MoveBlock[2][2]が中心、そこに合わせてblockの形を生成

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
void EntryBlock(short Data[][BLOCK_NUM]);
void Update(COORD* Position, short Rotate, bool End);
void Draw();

short Map[MAP_HEIGHT][MAP_WIDTH];
short Zangai[MAP_HEIGHT][MAP_WIDTH];
short SaveZangai[MAP_HEIGHT][MAP_WIDTH];

COORD FirstPosition;
COORD NewPosition;
short RotateDir;
short SaveRotate;
const short MovingValue = 1;
short MoveBlock[BLOCK_NUM][BLOCK_NUM];
short EntryBlockNum;

short EntryBlockTable[MAX_BLOCK_TABLE];

bool PushRotateFlag;
bool PushLeftRightFlag;
bool PushDownFlag;
bool PushReturnFlag;
bool FirstDrawFlag;
bool EndFlag;

BYTE Key[MAX_PATH];

#endif /* __TETRIS_H__ */

