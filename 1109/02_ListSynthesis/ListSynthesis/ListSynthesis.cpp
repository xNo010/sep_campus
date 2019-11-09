#include "ListSynthesis.h"

int main()
{
	int32_t NumList[10] =
	{
		1, 2, 4, 10, 3, 6, 0, 1, 7, 8
	};
	string CharList[10] =
	{
		"d", "v", "q", "to", "e", "z", "k", "m", "p", "s"
	};

	// 二つのリストの合成
	printf("リスト合成\n");
	Synthesis(NumList, CharList);

    std::cout << "Hello World!\n";
}

void Synthesis(int32_t List01[10], string List02[10])
{
	string temp = "";
	char CharTemp[30];
	for (short i = 0; i < 10; i++)
	{
		sprintf_s(CharTemp, "%d", List01[i]);
		temp += CharTemp + List02[i];
	}
	printf("合成結果:%s\n", temp.c_str());
}