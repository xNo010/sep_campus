#include <iostream>

void Fibonacci(int32_t FirstNum, int32_t NextNum, int32_t AnyTerm)
{
	int32_t save, temp;

	temp = FirstNum;
	if (AnyTerm == 0)
	{
		printf("%d\n", temp);
		return;
	}

	save = NextNum;
	temp = FirstNum + NextNum;
	NextNum = temp;
	FirstNum = save;

	if (AnyTerm != 0)
	{
		Fibonacci(FirstNum, NextNum, AnyTerm - 1);
	}
}

int main()
{
	printf("11番目～20番目のフィボナッチ\n");
	for (int32_t i = 11; i <= 20; i++)
	{
		Fibonacci(0, 1, i - 1);
	}
}