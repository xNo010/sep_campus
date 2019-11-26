#include <iostream>

int main()
{
	char strings[_MAX_PATH] = "";
	char temp[_MAX_PATH] = "";
	int32_t Length = 0;

	scanf_s("%100s", strings, 100);

	while (strings[Length] != NULL)
	{
		Length++;
	}

	/*for (int32_t i = Length - 1; i >= 0; i--)
	{
		printf("%c", strings[i]);
	}*/
	for (Length--; Length >= 0; Length--)
	{
		printf("%c", strings[Length]);
	}
}