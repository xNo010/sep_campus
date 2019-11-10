#pragma once
#ifndef __MAZE02_H__
#define __MAZE02_H__

#include "CommonInfo.h"

void Maze02();
void Initialize_02();
void PathCreate_02(short Map[][MAP_SIZE_X], COORD Position);
void StartFixed_02(short Map[][MAP_SIZE_X]);
void GoalFixed_02(short Map[][MAP_SIZE_X], COORD Position);

bool CheckIfCreatable(short Map[][MAP_SIZE_X], COORD CheckPos);

#endif /* __MAZE02_H__ */