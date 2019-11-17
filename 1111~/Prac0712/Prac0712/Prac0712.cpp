#include <iostream>

int main()
{
	char str[] = "Hello World";
	int32_t len = 0;

	// strの要素を逆順にする
	while (str[len] != 0)
	{
		len++;
	}

	// ずるいかも
	//char save[_MAX_PATH] = "";
	//for (int32_t i = 0; i < len; i++)
	//{
	//	save[i] = str[i];
	//}
	//for (int32_t i = len; i > 0; i--)
	//{
	//	str[len - i] = save[i - 1];
	//}

	char t;

	for (int32_t i = 0; i < (len / 2); i++)
	{
		t = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = t;
	}

	printf("%s\n", str);
}