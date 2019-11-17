#include <iostream>

void TriangleCreation(int32_t size)
{
	for (int32_t i = 0; i < size; i++)
	{
		for (int32_t j = 0; j <= i; j++)
		{
			printf("$");
		}
		printf("\n");
	}
}

int main()
{
	printf("サイズ3\n");
	TriangleCreation(3);
	printf("サイズ4\n");
	TriangleCreation(4);
	printf("サイズ5\n");
	TriangleCreation(5);
}