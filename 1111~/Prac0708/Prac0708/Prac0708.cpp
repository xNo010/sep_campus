#include <iostream>

int main()
{
	char str[_MAX_PATH] = "";
	int32_t offset = 'a' - 'A';

	scanf_s("%50s", str, 50);

	int32_t Len = 0;

	while (str[Len] != NULL)
	{
		Len++;
	}

	for (int32_t i = 0; i < Len; i++)
	{
		// 小文字かどうか
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= offset;
		}
	}

	printf("%s", str);
}