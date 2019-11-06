#include "yamanote.h"

int main()
{
	srand(timeGetTime());

	// ----- 初期化 -----
	// 電車
	for (short i = 0; i < MaxDataNum; i++)
	{
		// 1～23
		Train[i].ID = i + 1;
		// 3～5
		if (i <= 10)
		{
			Train[i].LeftMoveTime = 3;
		}
		else if (i <= 20)
		{
			Train[i].LeftMoveTime = 4;
		}
		else
		{
			Train[i].LeftMoveTime = 5;
		}
	}

	// 乗車開始ID
	StartId = EndId = 0;

	// 移動数(1～23)
	MovingNum = 5;//rand() % MaxDataNum + 1;
	// 10以上はいったん10に固定
	if (MovingNum >= 10)
		MovingNum = 10;

	MovingNum = 0;

	// 周期数(23以降から1, 46から2...)

	// 総移動時間
	TotalMoveTime = 0;
	// 最短移動時間
	ShortestMoveTime = 0;

	// ----- 実行 -----
	// IDの指定
	StartId = 4;
	EndId = 10;

	// 乗車IDと降車IDの表示
	std::cout << "乗車ID:" << StartId << "\n" << "降車ID:" << EndId << "\n";

	//std::cout << "Train[3].ID:4から、Train[" << SaveNumber << "].ID:" << SaveNumber + 1 << "までの到着時間は" << TotalMoveTime << "分です。\n\n";

	// 計算処理
	// ID/配列番号を毎回保存
	short SaveNumber = -1;

	// 反時計回り

	// 反時計のStartIdからEndIdは
	// 最大駅数からStartIdを引くと出てくる数分回す?
	MovingNum = StartId - MaxDataNum;
	for (short i = StartId; i > MovingNum; i--)
	{
		// 配列番号の保存
		SaveNumber = i - 1;
		if (SaveNumber < 0)
		{
			SaveNumber += MaxDataNum;
		}
		TotalMoveTime += Train[SaveNumber].LeftMoveTime;
	}

	// 反時計回り移動時間の表示
	printf("反時計回りの場合:移動時間は%d分です。\n", TotalMoveTime);

	// 一旦最短移動時間として格納
	ShortestMoveTime = TotalMoveTime;
	// 合計時間を初期化
	TotalMoveTime = 0;

	// 時計回り
	MovingNum = EndId;
	for (short i = StartId; i < MovingNum; i++)
	{
		// 配列番号の保存
		SaveNumber = i - 1;
		if (SaveNumber >= MaxDataNum)
		{
			SaveNumber -= MaxDataNum;
		}
		TotalMoveTime += Train[SaveNumber].LeftMoveTime;
	}

	// 時計回り移動時間の表示
	printf("時計回りの場合:移動時間は%d分です。\n", TotalMoveTime);

	// どちらが最短か決定
	if (ShortestMoveTime > TotalMoveTime)
	{
		ShortestMoveTime = TotalMoveTime;
	}
	
	// 最短移動時間の表示
	printf("乗車ID:%dから降車ID:%dまでの最短移動時間は%d分です。\n", StartId, EndId, ShortestMoveTime);

	// 移動時間の表示
	std::cout << "次の駅への移動時間(時計回り基準)\n";
	for (short i = 0; i < MaxDataNum; i++)
	{
		std::cout << "Train[" << i << "].ID:" << i + 1 << " " << Train[i].LeftMoveTime << "分\n";
	}
}