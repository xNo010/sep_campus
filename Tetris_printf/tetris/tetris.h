#pragma once
#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm.lib")

#define MAP_WIDTH	10
#define MAP_HEIGHT	12
#define BLOCK_NUM	4

enum EBlock
{
	None,
	Exist
};

bool Initialize();
void UpdateBlock(short NewPosition);
void Draw();

short Map[MAP_HEIGHT][MAP_WIDTH];
short Zangai[MAP_HEIGHT][MAP_WIDTH];
short MoveBlock[BLOCK_NUM][BLOCK_NUM];
short Position;
short NewPosition;
short UpdateCount;

bool PushFlag;
bool DrawFlag;
bool UpdateFlag;
bool EndFlag;

BYTE Key[MAX_PATH];

#endif /* __TETRIS_H__ */

