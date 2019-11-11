#include <iostream>

int main()
{
	int32_t MidtermExPoint, TermEndExPoint;

	printf("二つの試験から合格かどうか判定します(0点～100点満点)\n");
	std::cout << "中間試験の点数は:";
	std::cin >> MidtermExPoint;
	std::cout << "期末試験の点数は:";
	std::cin >> TermEndExPoint;

	if ((MidtermExPoint >= 60 && TermEndExPoint >= 60) ||
		MidtermExPoint + TermEndExPoint >= 130 ||
		MidtermExPoint + TermEndExPoint >= 100 && (MidtermExPoint >= 90 || TermEndExPoint >= 90))
	{
		printf("合格\n");
	}
	else
	{
		printf("不合格\n");
	}
}