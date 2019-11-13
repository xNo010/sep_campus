#include <iostream>

int main()
{
	int32_t StrikeCount = 0;
	int32_t BallCount = 0;

	while (StrikeCount < 3 && BallCount < 4)
	{
		int32_t temp;
		std::cout << "ストライク or ボール(ストライク:1, ボール:2):";
		std::cin >> temp;

		if (temp == 1)
		{
			StrikeCount++;
		}
		else if (temp == 2)
		{
			BallCount++;
		}
		else
		{
			printf("異常な値です\n");
		}
	}

	printf("%dボール, %dストライク\n", BallCount, StrikeCount);
}