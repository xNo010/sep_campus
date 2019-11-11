#include <iostream>

enum ETestStatus
{
	Passing,
	CanBe,
	Excellent
};

int main()
{
	int32_t TextPoint;
	int32_t Status;

	printf("テストの点数の成績を表示します\n");
	std::cout << "点数入力:";
	std::cin >> TextPoint;
	std::cout << "どの成績を見るか(0:合否 1:出来たかどうか 2:優良判定):";
	std::cin >> Status;	

	switch (Status)
	{
	case ETestStatus::Passing:
		if (TextPoint >= 60)
		{
			printf("合格\n");
		}
		else
		{
			printf("不合格\n");
		}
		break;
	case ETestStatus::CanBe:
		if (TextPoint >= 80)
		{
			printf("たいへんよくできました。\n");
		}
		else if (TextPoint >= 60)
		{
			printf("よくできました。\n");
		}
		else
		{
			printf("ざんねんでした。\n");
		}
		break;
	case ETestStatus::Excellent:
		if (TextPoint >= 80)
		{
			printf("優\n");
		}
		else if (TextPoint >= 70)
		{
			printf("良\n");
		}
		else if (TextPoint >= 60)
		{
			printf("可\n");
		}
		else
		{
			printf("不可\n");
		}
		break;
	default:
		break;
	}
}