#pragma once
#ifndef __MAZE01_H__
#define __MAZE01_H__

#include "CommonInfo.h"

void Maze01();
void Initialize_01();
void WallCreation(short Map[][MAP_SIZE_X], short x, short y);
void StartFixed_01(short Map[][MAP_SIZE_X]);
void GoalFixed_01(short Map[][MAP_SIZE_X]);

#endif /* __MAZE01_H__ */