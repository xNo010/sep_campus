#include <iostream>

int main()
{
	int32_t temp = 0;
	bool IsPrime = false;

	printf("2以上の数値を入力させ、それを素因数分解します\n");

	std::cout << "数値入力:";
	std::cin >> temp;

	int32_t Save = 0;
	if (temp < 2)
		return 0;

	while (temp != 1)
	{
		for (int32_t i = 2; i < temp; i++)
		{
			if (temp % i == 0)
			{
				temp /= i;
				Save = i;
				IsPrime = false;
				break;
			}
			
			IsPrime = true;
		}

		if (IsPrime)
		{
			Save = temp;
			temp /= temp;
		}

		printf("%d ", Save);
	}

	printf("\n素因数分解終了\n");
}