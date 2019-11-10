#pragma once
#ifndef __YAMANOTE_H__
#define __YAMANOTE_H__

#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm.lib")

#define MaxDataNum 23	// データの最大数

struct TrainData	// データ
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