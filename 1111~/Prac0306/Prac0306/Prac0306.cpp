#include <iostream>

int main()
{
	int32_t x;
	int32_t Save = 0;
	int32_t MultCount = 0;

	while (MultCount < 3)
	{
		x = 10;
		Save = x;
		for (int32_t i = 0; i < MultCount; i++)
		{
			x *= Save;
		}
		printf("xの%d乗:%d\n", MultCount + 1, x);

		MultCount++;
	}
}