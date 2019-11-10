#include "ListMaxComb.h"

int main()
{
	int32_t NumList[MaxListNum] =
	{
		4, 5, 30, 7, 9, 2
	};

	// 最大桁数を保存
	int32_t MaxDigit = 10;

	// その組み合わせで最大値になるよう合成
	printf("リスト内で最大値となる組み合わせ\n");
	MaxComb(NumList, MaxDigit);
}

void MaxComb(int32_t List[MaxListNum], int32_t Digit)
{
	// 表示用
	string DispValue = "";

	// リスト内の数値桁数をすべて一桁にする処理
	int32_t OriginValue[MaxListNum] = { 0, };
	for (int32_t i = 0; i < MaxListNum; i++)
	{
		if (List[i] < Digit)
			continue;

		// 元の値を保存
		OriginValue[i] = List[i];
		List[i] = List[i] / Digit;
	}

	// ソートを行う
	int32_t temp = 0;
	for (int32_t i = 0; i < MaxListNum - 1; i++)
	{
		for (int32_t j = i + 1; j < MaxListNum; j++)
		{
			// 降順にソート
			if (List[i] < List[j])
			{
				temp = List[i];
				List[i] = List[j];
				List[j] = temp;
			}
		}
	}

	// stringに合成
	for (int32_t i = 0; i < MaxListNum; i++)
	{
		// 元の値をList[i]で割ってDigitとなるものは、元の値に戻す
		for (int32_t j = 0; j < MaxListNum; j++)
		{
			if (OriginValue[j] / List[i] != Digit)
				continue;
			
			// 元の値に戻す
			List[i] = OriginValue[j];
		}

		// stringに合成
		DispValue += std::to_string(List[i]);
	}

	// 表示
	printf("最大となる組み合わせ%s\n", DispValue.c_str());
}