#include <iostream>

enum EWeekDay
{
	Sun,
	Mon,
	Tue,
	Wed,
	Thu,
	Fri,
	Sat
};

enum ESelectTime
{
	Am,
	Pm,
	Night
};

int main()
{
	int32_t WeekDay, SelectTime;

	printf("病院が開診か、休診かを調べます\n");
	std::cout << "曜日入力(0=日,1=月,2=火,3=水,4=木,5=金,6=土):";
	std::cin >> WeekDay;
	std::cout << "時間選択(0=午前,1=午後,2=夜間):";
	std::cin >> SelectTime;

	if (WeekDay == EWeekDay::Sun ||
		(WeekDay == EWeekDay::Tue && SelectTime == ESelectTime::Am) ||
		(WeekDay == EWeekDay::Wed && SelectTime == ESelectTime::Night) ||
		(WeekDay == EWeekDay::Fri && SelectTime == ESelectTime::Am) ||
		(WeekDay == EWeekDay::Tue && SelectTime >= ESelectTime::Pm))
	{
		printf("休診\n");
	}
	else
	{
		printf("開診\n");
	}
}