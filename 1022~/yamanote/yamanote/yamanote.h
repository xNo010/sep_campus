#pragma once
#ifndef __YAMANOTE_H__
#define __YAMANOTE_H__

#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm.lib")

#define MaxDataNum 23	// �f�[�^�̍ő吔

struct TrainData	// �f�[�^
{
	short ID;
	short LeftMoveTime;
};

TrainData Train[MaxDataNum];
short StartId;
short EndId;
short MovingNum;			// 
short TotalMoveTime;		// 
short ShortestMoveTime;	// 


#endif /* __YAMANOTE_H__ */