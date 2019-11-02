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
	Chip_Path,	// �ʘH
	Chip_Wall,	// ��
	Chip_Start,	// �X�^�[�g�n�_
	Chip_Goal,	// �S�[���n�_
	Chip_Max	// �`�b�v��ލő吔
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
short OpenDirection;	// �i�s����

bool itido;

void Draw();

#endif /* __COMMONINFO_H__ */