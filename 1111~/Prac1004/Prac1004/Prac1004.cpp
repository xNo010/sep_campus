#include <iostream>

struct GAKUSEI
{
	char name[20];
	int32_t TestPoint;
	char result;
};

void ResultDeter(GAKUSEI* pGaku)
{
	if (pGaku->TestPoint >= 80)
	{
		pGaku->result = 'A';
	}
	else if (pGaku->TestPoint >= 70)
	{
		pGaku->result = 'B';
	}
	else if (pGaku->TestPoint >= 60)
	{
		pGaku->result = 'C';
	}
	else
	{
		pGaku->result = 'D';
	}
}

int main()
{
	GAKUSEI gaku;

	printf("名前入力\n");
	scanf_s("%20s", &gaku.name, 20);
	printf("得点入力\n");
	scanf_s("%d", &gaku.TestPoint);

	ResultDeter(&gaku);

	printf("%s, %d, %c", gaku.name, gaku.TestPoint, gaku.result);
}