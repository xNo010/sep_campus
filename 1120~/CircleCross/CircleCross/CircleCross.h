#pragma once
#ifndef __CIRCLECROSS_H__
#define __CIRCLECROSS_H__

#include <iostream>
#include <Windows.h>

#define MAP_HEIGHT	6
#define MAP_WIDTH	6

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

int32_t Map[MAP_HEIGHT][MAP_WIDTH];
INPUTINFO Input;
int32_t HoriCheck;
int32_t VertCheck;
int32_t SaveTotal;
bool NowTurn;

void Initialize();
void InputFunc();
void Update();
bool Again();
void Draw();
bool SelectedCheck(int32_t Map[MAP_HEIGHT][MAP_WIDTH], int32_t Hori, int32_t Vert);
bool FinishCheck(int32_t Map[MAP_HEIGHT][MAP_WIDTH]);

#endif /* __CIRCLECROSS_H__ */