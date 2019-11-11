#include <iostream>

enum ESushi
{
	Salmon,
	Tuna,
	Shrimp,
	Eel,
	Mackerel
};

int main()
{
	int32_t SelectSushi;

	printf("鮨占い\n");
	std::cout << "カテゴリから鮨を選択(0:サーモン,1:マグロ,2:エビ,3:ウナギ,4:サバ):";
	std::cin >> SelectSushi;

	switch (SelectSushi)
	{
	case ESushi::Salmon:
		printf("サーモン：うまい\n");
		break;
	case ESushi::Tuna:
		printf("マグロ：何度も食える\n");
		break;
	case ESushi::Shrimp:
		printf("エビ：ぷりぷり\n");
		break;
	case ESushi::Eel:
		printf("ウナギ：ながい\n");
		break;
	case ESushi::Mackerel:
		printf("サバ：あまりくわない\n");
		break;
	default:
		break;
	}
}