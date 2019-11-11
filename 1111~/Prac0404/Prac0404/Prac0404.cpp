#include <iostream>

int main()
{
	int32_t x, y;

	std::cout << "x値入力:";
	std::cin >> x;
	std::cout << "y値入力:";
	std::cin >> y;

	if (x > y)
	{
		printf("yはxより小さい\n");
	}
	else
	{
		printf("yはxより大きい\n");
	}
}