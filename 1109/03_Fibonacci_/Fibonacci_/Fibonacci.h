#pragma once
#ifndef __FIBONACCI_H__
#define __FIBONACCI_H__

#include <Windows.h>
#include <iostream>
#include <stdlib.h>

uint64_t FirstNum;
uint64_t NextNum;

uint64_t Fibo[100];

void FibonacciCal(uint64_t First, uint64_t Next);

#endif /* __FIBONACCI_H__ */