#include <iostream>

int main()
{
	char a = 'a';
	char z = 'z';

	int32_t offset = z - a;

	int32_t ColumnCount = 0;

	printf("入力した列数に従いa～zを表示します\n");

	std::cout << "列数入力:";
	std::cin >> ColumnCount;

	for (int32_t i = 0; i <= offset; i++)
	{
		printf("%c", a + i);
		if (i != 0 && i % ColumnCount == 0)
		{
			printf("\n");
		}
	}
}