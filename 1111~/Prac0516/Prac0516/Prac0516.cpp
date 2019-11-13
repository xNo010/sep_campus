#include <iostream>

int main()
{
	printf("4以上の数を入力し、素数かどうか判定します\n");

	int32_t temp = 1;
	bool IsPrime = false;

	std::cout << "数値入力:";
	std::cin >> temp;
	
	int32_t Save = temp;
	if (temp < 4)
		return 0;

	for (int32_t i = 2; i < temp; i++)
	{
		if (temp % i == 0)
		{
			IsPrime = true;
			break;
		}
	}

	if (IsPrime)
	{
		printf("%dは素数です\n", temp);
	}
	else
	{
		printf("%dは素数ではないです\n", temp);
	}
}