#include <iostream>

int main()
{
	int32_t temp = 0;
	bool IsPrime = false;

	printf("4以上の数を入力し、素数かどうか判定します\n");

	std::cout << "数値入力:";
	std::cin >> temp;
	
	if (temp < 4)
		return 0;

	for (int32_t i = 2; i < temp; i++)
	{
		// 最大値迄ループし余りが0で割れた場合
		if (temp % i == 0)
		{
			// 素数でない
			IsPrime = false;
			break;
		}
		else
		{
			IsPrime = true;
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