#include <iostream>

char ConvIntoUpperCase(char ch)
{
	int32_t offset = 'a' - 'A';
	if (ch >= 'a' && 'z' >= ch)
	{
		return (ch - offset);
	}
	
	return ch;
}

int main()
{
	char str[100];
	int32_t len = 0;

	printf("文字列入力\n");
	scanf_s("%100s", str, 100);

	for (int32_t i = 0; str[i] != 0; i++)
	{
		str[i] = ConvIntoUpperCase(str[i]);
	}
	printf("%s\n", str);

	//while (str[len] != 0)
	//{
	//	len++;
	//}
	//
	//for (int32_t i = 0; i < len; i++)
	//{
	//	printf("%c", ConvIntoUpperCase(str[i]));
	//}
}