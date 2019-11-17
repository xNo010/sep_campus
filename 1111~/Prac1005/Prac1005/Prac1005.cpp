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
	GAKUSEI gaku[4];

	for (int32_t i = 0; i < 4; i++)
	{
		printf("%d人目の名前入力\n", i + 1);
		scanf_s("%20s", &gaku[i].name, 20);
		printf("%sの得点入力\n", gaku[i].name);
		scanf_s("%d", &gaku[i].TestPoint);

		ResultDeter(&gaku[i]);
	}

	for (int32_t i = 0; i < 4; i++)
	{
		printf("%s, %d, %c\n", gaku[i].name, gaku[i].TestPoint, gaku[i].result);
	}
}