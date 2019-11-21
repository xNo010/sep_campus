#pragma once
#ifndef __HANOI_H__
#define __HANOI_H__

#include <iostream>

#define MAP_WIDTH		42
#define MAP_HEIGHT		15
#define TOWER_WIDTH		11
#define TOWER_CENTER	((TOWER_WIDTH - 1) / 2)
#define TOWER_HEIGHT	10
#define MAX_TOWER		3
#define MAX_OBJECT		3
#define MIN_OBJSIZE		3
#define MAX_OBJSIZE		7
#define SIZEDIFF		2

enum ESizeCate
{
	Min = MIN_OBJSIZE,
	Middle = Min + SIZEDIFF,
	Max = MAX_OBJSIZE,
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

struct OBJECTINFO
{
	POSITION	InitPos;
	POSITION	Pos;
	int32_t		Size;
	bool		Data[MAX_OBJSIZE];
};

struct TOWERINFO
{
	POSITION Pos;
	int32_t Flag;
	bool Data[TOWER_HEIGHT][TOWER_WIDTH];
};

void Initialize();
void Execute();
void Update(int32_t BeforeNumber, int32_t MoveNumber);
void Draw();

bool MapData[MAP_HEIGHT][MAP_WIDTH];
TOWERINFO Tower[MAX_TOWER];
OBJECTINFO Object[MAX_OBJECT];
int32_t BeforeNumber;
int32_t MoveNumber;


#endif /* __HANOI_H__ */
