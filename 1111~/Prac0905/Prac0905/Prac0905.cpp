#include <iostream>

void DrawTriangle(int32_t size, char symbol)
{
	for (int32_t i = 0; i < size; i++)
	{
		for (int32_t j = 0; j <= i; j++)
		{
			printf("%c", symbol);
		}
		printf("\n");
	}
}

int main()
{
	printf("サイズ:3・記号:&\n");
	DrawTriangle(3, '&');
	printf("サイズ:5・記号:#\n");
	DrawTriangle(5, '#');
	printf("サイズ:8・記号:$\n");
	DrawTriangle(8, '$');
}