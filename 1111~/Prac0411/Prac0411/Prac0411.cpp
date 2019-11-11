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

	printf("その月の祝日を表示します\n");
	std::cout << "月:";
	std::cin >> SelectMonth;

	if (SelectMonth == EMonth::Jun || SelectMonth == EMonth::Aug)
	{
		printf("祝日なし\n");
	}
	if (SelectMonth > EMonth::Dec)
	{
		printf("そんな月ない\n");
	}

	switch (SelectMonth)
	{
	case EMonth::Jan:
		printf("元日、成人の日\n");
		break;
	case EMonth::Feb:		
		printf("建国記念の日\n");
		break;
	case EMonth::Mar:
		printf("春分の日\n");
		break;
	case EMonth::Apr:
		printf("昭和の日\n");
		break;
	case EMonth::May:
		printf("憲法記念日、みどりの日、こどもの日\n");
		break;
	case EMonth::Jul:
		printf("海の日\n");
		break;
	case EMonth::Sep:
		printf("敬老の日、秋分の日\n");
		break;
	case EMonth::Oct:
		printf("体育の日\n");
		break;
	case EMonth::Nov:
		printf("文化の日、勤労感謝の日\n");
		break;
	case EMonth::Dec:
		printf("天皇誕生日\n");
		break;
	default:
		printf("例外\n");
		break;
	}
}