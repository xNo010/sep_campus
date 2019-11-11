#include <iostream>

bool HelloWorld();

int main()
{
	if (!HelloWorld())
	{
		printf("世界にあいさつ出来てません\n");
	}
}

bool HelloWorld()
{
	const char* Hello = "Hello World\n";
	if (Hello != "Hello World\n")
	{
		return false;
	}

	printf(Hello);

	return true;
}