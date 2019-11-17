#include <iostream>

int main()
{
	char strings[_MAX_PATH] = "";

	scanf_s("%10s", strings, 10);

	int32_t Length = 0;

	while (strings[Length] != NULL)
	{
		Length++;
	}

	std::cout << "Length = " << Length;
}