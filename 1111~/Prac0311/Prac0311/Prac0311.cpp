#include <iostream>

int main()
{
	float int01, int02;
	std::cout << "一つ目の整数値入力:";
	std::cin >> int01;
	std::cout << "二つ目の整数値入力:";
	std::cin >> int02;

	printf("平均値:%d\n", (int)((int01 + int02) / 2));
}
