#include <iostream>

int main()
{
	int32_t YearsOld;

	printf("あなたが生まれてからおおよその日数を表示します\n");
	std::cout << "貴方の年齢は？:";
	std::cin >> YearsOld;

	printf("おおよそ%d日\n", 365 * YearsOld);
}