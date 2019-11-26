#include <iostream>

int main()
{
	char str[_MAX_PATH] = "";
	int Len = 0;
	int32_t UpperCaseCount = 0;

	scanf_s("%100s", str, 100);

	while (str[Len] != NULL)
	{
		Len++;
	}

	for (int32_t i = 0; i < Len; i++)
	{
		if (!(str[i] >= 'a' && str[i] <= 'z'))
		{
			str[UpperCaseCount++] = str[i];
		}
	}

	str[UpperCaseCount] = 0;

	printf("%s", str);
}