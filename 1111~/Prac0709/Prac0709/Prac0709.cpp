#include <iostream>

int main()
{
	char str[_MAX_PATH] = "";

	for (int32_t i = 0; i < 20; i++)
	{
		str[i * 4] = 'S';
		str[i * 4 + 1] = 'P';
		str[i * 4 + 2] = 'A';
		str[i * 4 + 3] = 'M';
	}

	printf("%s", str);
}