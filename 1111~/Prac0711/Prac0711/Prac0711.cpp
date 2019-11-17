#include <iostream>

int main()
{
	char    str1[100] = "Hello ";
	char    str2[] = "World";

	// str1にstr2を結合する
	int len1 = 0, len2 = 0;
	while (str1[len1] != 0)
	{
		len1++;
	}
	while (str2[len2] != 0)
	{
		len2++;
	}

	for (int32_t i = len1; i < len1 + len2; i++)
	{
		str1[i] = str2[i - len1];
	}

	printf("%d, %d\n", len1, len2);

	printf("%s\n", str1);
}