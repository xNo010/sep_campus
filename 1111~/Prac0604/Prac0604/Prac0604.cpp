#include <iostream>

int main()
{
	int32_t Value[10] = { 0, };
	int32_t InputCount = 0;
	int32_t temp = 0;

	while (InputCount < 10 && temp <= 100)
	{
		scanf_s("%d", &Value[InputCount]);
		temp += Value[InputCount];

		InputCount++;
	}

	for (int32_t i = 0; i < 10; i++)
	{
		if (Value[i] == 0)
			continue;

		printf("%d ", Value[i]);
	}
}