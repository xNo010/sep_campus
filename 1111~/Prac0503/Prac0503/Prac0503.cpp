#include <iostream>

int main()
{
	int32_t Save = 0;
	int32_t MultCount = 0;

	printf("2の1～8乗の表示\n");

	while (MultCount < 8)
	{
		int32_t Int01 = 2;
		Save = Int01;
		for (int32_t i = 0; i < MultCount; i++)
		{
			Int01 *= Save;
		}
		printf("2の%d乗:%d\n", MultCount + 1, Int01);


		MultCount++;
	}
}