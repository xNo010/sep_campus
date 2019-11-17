#include <iostream>

struct MonthPreci
{
	int32_t Month;
	int32_t Preci;
};

int main()
{
	struct MonthPreci MP;
	printf("何月から\n");
	scanf_s("%d", &MP.Month);

	for (int32_t i = MP.Month; i < MP.Month + 4; i++)
	{
		int32_t save = i;
		if (save > 12)
			save = i - 12;

		printf("%2d月の降水量は\n", save);
		scanf_s("%d", &MP.Preci);
		printf("%2d月　%2dmm\n", save, MP.Preci);
	}
}