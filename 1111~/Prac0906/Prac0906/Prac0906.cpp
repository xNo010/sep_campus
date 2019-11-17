#include <iostream>

void PartialMult(int32_t NumSteps)
{
	printf("%dの段", NumSteps);
	for (int32_t i = 0; i < 9; i++)
	{
		printf("%2d ", NumSteps * (i + 1));
	}
	printf("\n");
}

int main()
{
	for (int32_t i = 0; i < 9; i++)
	{
		PartialMult(i + 1);
	}
}