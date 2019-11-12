#include <iostream>

int main()
{
	int32_t Point = 0;
	int32_t GPoint = 0;
	int32_t TPoint = 0;

	for (int32_t i = 1; i <= 9; i++)
	{
		std::cout << i << "回表、巨人の得点は？:";
		std::cin >> Point;
		GPoint += Point;
		
		std::cout << i << "回裏、阪神の得点は？:";
		std::cin >> Point;
		TPoint += Point;
	}

	printf("巨人 : %d, 阪神 : %d\n", GPoint, TPoint);
	if (GPoint > TPoint)
	{
		printf("巨人の勝ち\n");
	}
	else
	{
		printf("阪神の勝ち\n");
	}
}