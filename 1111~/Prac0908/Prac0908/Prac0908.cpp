#include <iostream>

bool IsPrime(int32_t value)
{
	for (int32_t i = 2; i < value; i++)
	{
		if (value % i == 0)
		{
			return false;
		}
	}

	return true;
}

int main()
{
	int32_t count = 0;

	for (int32_t i = 10000; i < 10100; i++)
	{
		if (!IsPrime(i))
			continue;

		printf("%d, ", i);
		if (++count == 6)
		{
			printf("\n");
			count = 0;
		}
	}
}