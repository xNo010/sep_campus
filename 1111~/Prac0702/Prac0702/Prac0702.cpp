#include <iostream>

int main()
{
	char a = 'A';

	int32_t offset = 'a' - 'A';

	printf("元の文字:%c\n", a);

	a += offset;
	printf("変換後文字:%c\n", a);
}