#include "Lead100.h"

int main()
{
	srand(timeGetTime());

	int32_t List[MAX_LIST] =
	{
		1, 2, 3, 4, 5, 6, 7, 8, 9
	};

	// 合計100を求める
	Lead100(List);
}

void Lead100(int32_t List[MAX_LIST])
{
	// 何個結果を表示したいか
	const int32_t MaxDisp = 10;
	// 100を導けた回数を保存
	int32_t LeadCount = 0;

	// 表示用
	string Dispstr[MaxDisp] = { "" };

	int32_t SaveRandNum = 2;

	while (LeadCount < MaxDisp)
	{
		Dispstr[LeadCount] = "";

		// 乱数登録
		int32_t RandNum = 0;

		// 元の値の保存
		int32_t OriginValue[MAX_LIST] = { 0, };
		// 合計値の保存
		int32_t temp = 0;

		// 後でstringに入れる用
		char SaveChar[MAX_PATH] = {  };
		// ループ毎保存用
		char buffer[MAX_PATH] = "\0";

		// リスト分回して100を導く
		for (int32_t i = 0; i < MAX_LIST; i++)
		{
			// 最初に元の値保存
			OriginValue[i] = List[i];

			// 足すか、引くか、くっつくか(かける)
			// 最初は引くパターンを/最後はかけるパターンを排除
			if (i == 0 || i == MAX_LIST - 1)
			{
				RandNum = rand() % 2;
	
				if (i == 0 && RandNum == 1)
				{
					RandNum = 2;
				}
			}
			// それ以外は普通に選択
			else
			{
				RandNum = rand() % 3;
			}

			// 以前のリストを見て、掛けられていた場合必ず足す処理
			if (i != 0 && List[i - 1] / 10 == OriginValue[i - 1])
			{
				RandNum = 0;
			}
	
			switch (RandNum)
			{
			case 0:					// 足す
				temp += List[i];
				if (SaveRandNum != 2)
				{
					sprintf_s(buffer, "+%d", List[i]);
				}
				else
				{
					// 前のやつとくっつける為
					sprintf_s(buffer, "%d", List[i]);
				}
				// 合成
				strcat_s(SaveChar, buffer);

				// 最初の時だけ+が付いたとき消したい
				//if (i == 0)
				//{
				//	// string型に送り0を消す処理
				//	Dispstr[LeadCount] = SaveChar;
				//	Dispstr[LeadCount].erase(Dispstr[LeadCount].find('+'), 1);
				//	strcpy_s(SaveChar, Dispstr[LeadCount].c_str());
				//}
				break;
			case 1:					// 引く
				temp -= List[i];
				sprintf_s(buffer, "-%d", List[i]);
				// 合成
				strcat_s(SaveChar, buffer);
				break;
			case 2:					// かける
				List[i] *= 10;
				temp += List[i];
				if (SaveRandNum != 2)
				{
					sprintf_s(buffer, "+%d", List[i]);
				}
				else
				{
					// 前のやつとくっつける為
					sprintf_s(buffer, "%d", List[i]);
				}

				// 合成
				strcat_s(SaveChar, buffer);

				// string型に送り0を消す処理
				Dispstr[LeadCount] = SaveChar;
				Dispstr[LeadCount].erase(Dispstr[LeadCount].find('0'), 1);
				strcpy_s(SaveChar, Dispstr[LeadCount].c_str());
			default:
				break;
			}

			// 前回の乱数を持つ
			SaveRandNum = RandNum;
		}
	
		// 結果100を求められてたら
		if (temp == 100)
		{
			sprintf_s(buffer, " = %d", temp);
			strcat_s(SaveChar, buffer);
			Dispstr[LeadCount] = SaveChar;
			LeadCount++;
		}

		// リストを元の値に戻すループ
		for (int32_t i = 0; i < MAX_LIST; i++)
		{
			List[i] = OriginValue[i];
		}
	}

	// 表示
	for (int32_t i = 0; i < MaxDisp; i++)
	{
		printf("計算式と結果:%s\n", Dispstr[i].c_str());
	}

}