#include <iostream>

int main()
{
	int32_t a = 0;
	int32_t Win = 0;
	int32_t Lose = 0;

	printf("10回0か1を入力し、勝ち負けを決定します\n");
	for (int32_t i = 1; i <= 10; i++)
	{
		std::cout << i << "回目の入力(0:負け, 1:勝ち):";
		std::cin >> a;
		switch (a)
		{
		case 0:
			Win++;
			break;
		case 1:
			Lose++;
			break;
		default:
			printf("ちゃんと入力してください\n");
			break;
		}
	}

	printf("勝ち数:%d, 負け数:%d", Win, Lose);
}