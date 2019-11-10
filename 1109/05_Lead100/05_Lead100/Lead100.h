#pragma once
#ifndef __LEAD100_H__
#define __LEAD100_H__

#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <string>

#pragma comment(lib,"winmm.lib")

using namespace std;

#define MAX_LIST 9

void Lead100(int32_t List[MAX_LIST]);

#endif /* __LEAD100_H__ */