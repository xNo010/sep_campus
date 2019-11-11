#include <iostream>
enum EMonth
{
	None,
	Jan,
	Feb,
	Mar,
	Apr,
	May,
	Jun,
	Jul,
	Aug,
	Sep,
	Oct,
	Nov,
	Dec
};

int main()
{
	int32_t SelectMonth;

	printf("その月の日数を表示します\n");
	std::cout << "月:";
	std::cin >> SelectMonth;

	if (SelectMonth > EMonth::Dec || SelectMonth <= EMonth::None)
	{
		printf("にゅうりょくがまちがっています\n");
	}

	switch (SelectMonth)
	{
	case EMonth::Feb:	// 2
		printf("28日\n");
		break;
	case EMonth::Jan:	// 1
	case EMonth::Mar:	// 3
	case EMonth::May:	// 5
	case EMonth::Jul:	// 7
	case EMonth::Aug:	// 8
	case EMonth::Oct:	// 10
	case EMonth::Dec:	// 12
		printf("31日\n");
		break;
	case EMonth::Apr:	// 4
	case EMonth::Jun:	// 6
	case EMonth::Sep:	// 9
	case EMonth::Nov:	// 11
		printf("30日\n");
		break;
	default:
		break;
	}
}