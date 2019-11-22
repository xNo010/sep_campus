#pragma once
#ifndef __CIRCLECROSS_H__
#define __CIRCLECROSS_H__

#include <iostream>
#include <Windows.h>

#define MAP_HEIGHT	6
#define MAP_WIDTH	6
#define HALF_MAP_HEIGHT MAP_HEIGHT / 2
#define HALF_MAP_WIDTH	MAP_WIDTH / 2

enum EChipCate
{
	None,
	Square,
	Circle,
	Cross,
	VertLine,
	HoriLine,
	Max
};

struct INPUTINFO
{
	char HoriSelect;
	int32_t VertSelect;
};

int32_t CheckMap[HALF_MAP_HEIGHT][HALF_MAP_WIDTH];
int32_t DrawMap[MAP_HEIGHT][MAP_WIDTH];
INPUTINFO Input;
int32_t HoriPos;
int32_t VertPos;
int32_t WhatHandCount;
int32_t TotalSelected;
bool NowTurn;

void Initialize();
void InputFunc();
void Update();
bool Again();
void Draw();
bool SelectedCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], int32_t Hori, int32_t Vert);
bool FinishCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], int32_t WhatHandCount);
bool MatrixCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], const int32_t FinishCount);
bool ObliqueCheck(int32_t Map[HALF_MAP_HEIGHT][HALF_MAP_WIDTH], const int32_t FinishCount);

#endif /* __CIRCLECROSS_H__ */