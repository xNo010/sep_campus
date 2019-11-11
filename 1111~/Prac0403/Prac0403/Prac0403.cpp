#include <iostream>

int main()
{
	int32_t Int01, Int02;

	std::cout << "一つ目の値入力:";
	std::cin >> Int01;
	std::cout << "二つ目の値入力:";
	std::cin >> Int02;

	if (Int01 > Int02)
	{
		printf("大きい値:%d\n", Int01);
	}
	else
	{
		printf("大きい値:%d\n", Int02);
	}
}