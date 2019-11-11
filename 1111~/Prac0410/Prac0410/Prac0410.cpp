#include <iostream>

int main()
{
	int32_t x, y;

	std::cout << "整数値x入力:";
	std::cin >> x;
	std::cout << "整数値y入力:";
	std::cin >> y;

	if (x < y && (x % 2 == 0 && y % 2 == 0))
	{
		printf("xはyより小さく、かつ、xとyはともに偶数である。\n");
	}
	if (x == y && (x > 0 && y > 0))
	{
		printf("xとyは等しく、かつ、負の数である。\n");
	}
	if (x < y || x % 2 == 0)
	{
		printf("xはyより小さい、または、xは偶数である。\n");
	}
	if ((x <= 10 || x >= 100) && (y >= 10 || y <= 100))
	{
		printf("xは10以下または100以上で、かつ、yは10以上または100以下である。\n");
	}
	if (x >= 0 && y >= 0)
	{
		printf("xもyも負の数である、ではない。\n");
	}
}