#pragma once
#ifndef __COMMONINFO_H__
#define __COMMONINFO_H__

#include <windows.h>
#include <iostream>
#include <stdlib.h>

#pragma comment(lib,"winmm.lib")

#define MAP_SIZE_X 30
#define MAP_SIZE_Y 30

enum EChip
{
	Chip_Path,	// 通路
	Chip_Wall,	// 壁
	Chip_Start,	// スタート地点
	Chip_Goal,	// ゴール地点
	Chip_Max	// チップ種類最大数
};

enum EDirection
{
	Dir_Left,
	Dir_Up,
	Dir_Right,
	Dir_Down,
	Dir_Max
};

short MazeMap[MAP_SIZE_Y][MAP_SIZE_X];
short SearchedMap[MAP_SIZE_Y][MAP_SIZE_X];
COORD FirstPosition;
COORD SavePosition;
COORD NextPosition;
short OpenDirection;	// 進行方向

bool itido;

void Draw();

#endif /* __COMMONINFO_H__ */