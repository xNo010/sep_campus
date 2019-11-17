#include <iostream>

int main()
{
	int32_t FirstNum = 0;
	int32_t NextNum = 1;
	int32_t Save = 0;
	int32_t temp = 0;

	printf("1000までのフィボナッチ数列\n");

	for (; ;)
	{
		Save = NextNum;
		temp = FirstNum + NextNum;
		NextNum = temp;
		FirstNum = Save;

		if (temp >= 1000)
		{
			break;
		}

		printf("%d, ", temp);
	}
}