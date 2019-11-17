#include <iostream>

int main()
{
	char str[] = "hello world";
	int32_t offset = 'a' - 'A';

	// 'h'と'w'を大文字にする
	*str -= offset;
	*(str + 6) -= offset;

	printf("%s\n", str);
}