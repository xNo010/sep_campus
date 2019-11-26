#include <iostream>

int main()
{
	int32_t offset = 'a' - 'A';
	char a = 'A';

	printf("元の文字:%c\n", a);

	a += offset;
	printf("変換後文字:%c\n", a);
}