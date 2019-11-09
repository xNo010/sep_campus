#include "Fibonacci.h"

int main()
{
	ZeroMemory(Fibo, sizeof(Fibo));

	FirstNum = 0;
	NextNum = 1;

	// フィボナッチ数列を計算
	printf("フィボナッチ数列計算\n");
	FibonacciCal(FirstNum, NextNum);

    std::cout << "Hello World!\n";
}

void FibonacciCal(uint64_t First, uint64_t Next)
{
	uint64_t temp = 0;

	for (int32_t i = 0; i < 100; i++)
	{
		if (i == 0)
		{
			Fibo[i] = First;
		}
		else if (i == 1)
		{
			Fibo[i] = Next;
		}
		else
		{
			temp = First + Next;
			Fibo[i] = temp;
			First = Next;
			Next = temp;
		}

		printf("リスト[%d]:%llu\n", i, Fibo[i]);
	}
}