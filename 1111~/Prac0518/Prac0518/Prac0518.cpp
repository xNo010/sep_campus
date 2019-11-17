#include <iostream>

int main()
{
	printf("九九表\n");

	for (int32_t i = 1; i <= 9; i++)
	{
		printf("%dの段:", i);
		for (int32_t j = 1; j <= 9; j++)
		{
			printf("%2d ", i * j);
		}
		printf("\n");
	}
}