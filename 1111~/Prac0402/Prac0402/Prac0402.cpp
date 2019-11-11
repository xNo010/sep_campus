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
		printf("xはyより大きい\n");
	}
	else
	{
		printf("xはyより小さい\n");
	}
}