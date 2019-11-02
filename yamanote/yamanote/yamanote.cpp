#include <windows.h>
#include <iostream>

#pragma comment(lib,"winmm.lib")

#define MaxDataNum 23	// データの最大数

struct TrainData	// データ
{
	int ID;
	int LeftMoveTime;
};

TrainData Train[MaxDataNum];
int MovingNum;			// 
int WhereDir;			// 左:-1 右:+1
int TotalMoveTime;		// 
int ShortestMoveTime;	// 

int main()
{
	srand(timeGetTime());

	// ----- 初期化 -----
	// 電車
	for (int i = 0; i < MaxDataNum; i++)
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

	// 移動数(1～23)
	MovingNum = rand() % MaxDataNum + 1;
	// 10以上はいったん10に固定
	if (MovingNum >= 10)
		MovingNum = 10;

	// 周期数(23以降から1, 46から2...)

	// 移動方向(左:-1 右:+1)
	// 最終的にどちらの方向が最短かを見るのでここいらん
	//WhereDir = rand() % 2;
	//if (WhereDir == 0)
	//	WhereDir = -1;
	WhereDir = 0;

	// 総移動時間
	TotalMoveTime = 0;
	// 最短移動時間
	ShortestMoveTime = 0;

	// ----- 実行 -----
	// IDの指定

	// 移動数表示
	std::cout << "移動数:" << MovingNum << "\n\n";

	// 反時計回り(反時計回り)の場合
	if (WhereDir == -1)
	{
		std::cout << "移動方向:反時計回り \n";

		// IDを毎回保存
		int SaveNumber = -1;

		// i = 0はない(最終的にidを指定させたい)
		for (int i = 3; i > 3 - MovingNum; i--)
		{
			SaveNumber = i;
			if (SaveNumber < 0)
			{
				// -23まではRollCountは0?
				//RollCount = i / MaxDataNum;
				SaveNumber += MaxDataNum /*RollCountが1以上なら+ (1, 2, 3) を足せるようにしたい*/;
			}
			TotalMoveTime += Train[SaveNumber].LeftMoveTime;
		}

		std::cout << "Train[3].ID:4から、Train[" << SaveNumber << "].ID:" << SaveNumber + 1 << "までの到着時間は" << TotalMoveTime << "分です。\n\n";
	}
	// 時計回り(時計回り)の場合
	else if (WhereDir == 1)
	{
		std::cout << "移動方向:時計回り \n";

		// IDを毎回保存
		int SaveNumber = -1;

		// i = 0はない(最終的にidを指定させたい)
		for (int i = 3; i < (3 + MovingNum); i++)
		{
			SaveNumber = i;
			if (SaveNumber >= MaxDataNum)
			{
				// +23まではRollCountは0?
				//RollCount = i / MaxDataNum;
				SaveNumber -= MaxDataNum;
			}
			TotalMoveTime += Train[SaveNumber].LeftMoveTime;
		}

		std::cout << "Train[3].ID:4から、Train[" << SaveNumber << "].ID:" << SaveNumber + 1 << "までの到着時間は" << TotalMoveTime << "分です。\n\n";
	}
	// 例外
	else
	{
		std::cout << "方向が決まっていません\n\n";
	}

	// 計算処理
	// ID/配列番号を毎回保存
	int SaveNumber = -1;

	// 反時計回り
	// i = 0はない(最終的にidを指定させたい)
	for (int i = 3; i > 3 - MovingNum; i--)
	{
		SaveNumber = i;
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

	// 時計回り
	// i = 0はない(最終的にidを指定させたい)
	for (int i = 3; i < (3 + MovingNum); i++)
	{
		SaveNumber = i;
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
	printf("最短移動時間は%d分です。\n", ShortestMoveTime);

	// 移動時間の表示
	std::cout << "1駅移動時間(時計回り基準)\n";
	for (int i = 0; i < MaxDataNum; i++)
	{
		std::cout << "Train[" << i << "].ID:" << i + 1 << "..." << Train[i].LeftMoveTime << '\n';
	}
}