#include <iostream>

int main()
{
	int32_t x, y;

	std::cout << "x値入力:";
	std::cin >> x;
	std::cout << "y値入力:";
	std::cin >> y;

	printf("和:%d\n", x + y);
	printf("差:%d\n", x - y);
	printf("積:%d\n", x * y);
	printf("商:%d\n", x / y);
	printf("余り:%d\n", x % y);
}
