#include "TotalCal.h"

int main()
{
	short NumberList[10]
	{
		4, 6, 3, 10, 3, 6, 25, 60, 2, 100
	};

	// for文での計算
	printf("for文計算\n\n");
	For_Cal(NumberList);

	// whileループでの計算
	printf("\nwhile文計算\n\n");
	While_Cal(NumberList);

	// 再帰処理での計算
	tempPass = 0;
	printf("\n再帰処理計算\n\n");
	Recur_Cal(NumberList, 10);
}

void For_Cal(short List[10])
{
	// 合計値格納
	short temp = 0;
	for (short i = 0; i < 10; i++)
	{
		temp += List[i];
		printf("現在の合計数値:%d\n", temp);
	}
}
void While_Cal(short List[10])
{
	short count = 0;
	short temp = 0;
	while (count < 10)
	{
		temp += List[count];
		printf("現在の合計数値:%d\n", temp);

		count++;
	}

}
void Recur_Cal(short List[10], short count)
{
	tempPass += List[--count];
	printf("現在の合計数値:%d\n", tempPass);

	if (count > 0)
	{
		Recur_Cal(List, count);
	}
}
