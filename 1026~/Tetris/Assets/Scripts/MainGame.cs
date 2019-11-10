using UnityEngine;
using System;
using System.Collections.Generic;

public class MainGame : MonoBehaviour
{
    public enum EChip
    {
        None,
        Exist,
        OutSide
    };

    public enum EBlockCategory
    {
        Square = 0,
        Stick,
        Convex,
        Z,
        InvZ,
        L,
        InvL,
        MaxNum
    };

    public enum EOffsetCoord
    {
        MoreMoreMin = -2,
        MoreMin,
        Min,
        Max = 5,
        MoreMax,
        MoreMoreMax,
    };

    private string[] prefabsName = new string[(short)EBlockCategory.MaxNum]
    {
        "Prefabs/SquareBlock",
        "Prefabs/StickBlock",
        "Prefabs/ConvexBlock",
        "Prefabs/ZBlock",
        "Prefabs/InverseZBlock",
        "Prefabs/LBlock",
        "Prefabs/InverseLBlock"
    };

    private short[] entryBlockTable;        // 表示ブロックテーブル

    private const short BLOCK_NUM_X = 5;    // 5*5で作られるmoveBlock用
    private const short BLOCK_NUM_Y = 5;    // 5*5で作られるmoveBlock用
    private const short MAPSIZE_X = 10;     // マップの縦要素数
    private const short MAPSIZE_Y = 25;     // マップの横要素数
    private short[,] map;                   // moveBlockが移動可能なマップ
    private short[,] remainBlock;           // moveBlockを送りマップに配置する
    private short[,] saveRemainBlock;       // RemainBlockを保存する

    private Vector2Int popCoord;            // 出現座標変数
    private Vector2Int nextCoord;            // 更新座標変数
    private short rotateDir;                // 回転情報変数
    private short saveRotate;               // 回転情報の保存変数
    private const short MOVING_VALUE = 1;   // 毎移動量
    private short[,] moveBlock;             // 動かすブロック
    private InputScript inputScript;        // 入力情報スクリプト
    private GameObject drawBlock;
    private GameObject outsideDrawBlock;

    [SerializeField]
    public GameObject mapBlockManage;
    
    //private Block blockScript;              // 更新された座標をセットする為のコンポーネント

    private EBlockCategory entryNumber;  // ランダムに生成される登録番号

    private bool pushRotateFlag;    // 回転キーを押したかどうか
    private bool pushLeftRightFlag; // 左右キーを押したかどうか
    private bool pushDownFlag;      // 下キーを押したかどうか
    private bool pushReturnFlag;    // エンターキーを押したかどうか
    private bool endFlag;           // 終わるかどうか

    private float turnCount;        // 回るタイミング

    void Start()
    {
        Initialize();

        inputScript = GameObject.Find("InputParent").GetComponent<InputScript>();

        // 指定座標に、キーを対応させたブロックを生成
    }

    void Update()
    {
        turnCount += Time.deltaTime;
        if (turnCount >= 1.0f)
        {
            turnCount = 0.0f;
            nextCoord.y += MOVING_VALUE;

            Execute(moveBlock, nextCoord, rotateDir, endFlag);
        }
    }

    void Initialize()
    {
        // アセットの読み込み
        drawBlock = (GameObject)Resources.Load("Prefabs/OneBlock");
        outsideDrawBlock = (GameObject)Resources.Load("Prefabs/RedOneBlock");

        // 配列の初期化
        map = new short[MAPSIZE_Y, MAPSIZE_X];
        remainBlock = new short[MAPSIZE_Y, MAPSIZE_X];
        saveRemainBlock = new short[MAPSIZE_Y, MAPSIZE_X];

        // 変数を初期化
        popCoord = Vector2Int.zero;
        nextCoord = Vector2Int.zero;
        rotateDir = 0;
        saveRotate = 4;
        entryNumber = 0;
        turnCount = 0.0f;

        pushRotateFlag = false;
        pushLeftRightFlag = false;
        pushDownFlag = false;
        pushReturnFlag = false;
        endFlag = false;

        // mapのカメラ範囲外のところを設定
        for (short i = 0; i < MAPSIZE_Y - 20; i++)
        {
            for (short j = 0; j < MAPSIZE_X; j++)
            {
                map[i, j] = (short)EChip.OutSide;
            }
        }

        // 最初に何らかの形のブロックを作る
        EntryBlock(moveBlock);

        // 初期マップ描画
        Draw();
    }

    void EntryBlock(short[,] blockData)
    {
        blockData = new short[BLOCK_NUM_Y, BLOCK_NUM_X];

        // 乱数で形を決定
        entryNumber = (EBlockCategory)UnityEngine.Random.Range((short)EBlockCategory.Square, (short)EBlockCategory.MaxNum);

        // デバッグ用
        entryNumber = EBlockCategory.Square;

        // ブロック生成
        // [2][2]が中心
        switch (entryNumber)
        {
            case EBlockCategory.Square:    // 四角
                blockData[1, 1] = (short)EChip.Exist;
                blockData[1, 2] = (short)EChip.Exist;
                blockData[2, 1] = (short)EChip.Exist;
                blockData[2, 2] = (short)EChip.Exist;
                break;
            case EBlockCategory.Stick:     // 棒(縦または横)
                for (short i = 1; i <= 4; i++)
                {
                    blockData[i, 2] = (short)EChip.Exist;
                }
                break;
            case EBlockCategory.Convex:    // 凸
                blockData[3, 2] = (short)EChip.Exist;
                blockData[2, 1] = (short)EChip.Exist;
                blockData[2, 2] = (short)EChip.Exist;
                blockData[2, 3] = (short)EChip.Exist;
                break;
            case EBlockCategory.Z:         // Z
                blockData[2, 1] = (short)EChip.Exist;
                blockData[2, 2] = (short)EChip.Exist;
                blockData[3, 2] = (short)EChip.Exist;
                blockData[3, 3] = (short)EChip.Exist;
                break;
            case EBlockCategory.InvZ:      // 逆Z
                blockData[2, 2] = (short)EChip.Exist;
                blockData[2, 3] = (short)EChip.Exist;
                blockData[3, 1] = (short)EChip.Exist;
                blockData[3, 2] = (short)EChip.Exist;
                break;
            case EBlockCategory.L:         // L
                blockData[1, 1] = (short)EChip.Exist;
                blockData[2, 1] = (short)EChip.Exist;
                blockData[2, 2] = (short)EChip.Exist;
                blockData[2, 3] = (short)EChip.Exist;
                break;
            case EBlockCategory.InvL:      // 逆L
                blockData[1, 3] = (short)EChip.Exist;
                blockData[2, 1] = (short)EChip.Exist;
                blockData[2, 2] = (short)EChip.Exist;
                blockData[2, 3] = (short)EChip.Exist;
                break;
            default:
                break;
        }

        // 回転情報初期化
        rotateDir = 0;
        // 初期位置の決定
        popCoord.x = 2;
        popCoord.y = 9;
        nextCoord = popCoord;

        // マップに配置
        for (short i = 0; i < BLOCK_NUM_Y; i++)
        {
            for (short j = 0; j < BLOCK_NUM_X; j++)
            {
                if (blockData[i, j] != (short)EChip.Exist)
                    continue;

                map[popCoord.y - i, popCoord.x + j] = blockData[i, j];
            }
        }

        //Execute(moveBlock, nextCoord, rotateDir, endFlag);
    }

    void Execute(short[,] blockData, Vector2Int newCoord, short rotate, bool end)
    {
        // --- マップの更新 ---
        // Mapをまっさらな状態に
        for (short i = 0; i < MAPSIZE_Y; i++)
        {
            for (short j = 0; j < MAPSIZE_X; j++)
            {
                if (i < 5)
                {
                    map[i, j] = (short)EChip.OutSide;
                }
                else
                {
                    map[i, j] = (short)EChip.None;
                }
            }
        }

        // moveBlockの座標更新
        blockData = new short[BLOCK_NUM_Y, BLOCK_NUM_X];
        // 先に回転処理
        // 0の時・-1(3)の時・1(-3)の時・2(-2)の時が存在
        // ブロックの種類によって端まで行く際の座標が決まる
        switch (entryNumber)
        {
            case EBlockCategory.Square:    // 四角
                // 移動制限
                if (rotate == -1 || rotate == 0 || rotate == 3)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMax;
                    }

                    Debug.Log(newCoord);
                }
                else if (rotate == -3 || rotate == 1 || Mathf.Abs(rotate) == 2)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }

                // 回転値同じなら回転処理は不要
                if (saveRotate == rotate)
                    break;
                // 回転による形状変化
                if (rotate == 0)
                {
                    blockData[1, 1] = (short)EChip.Exist;
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;

                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                }
                else if (rotateDir == -3 || rotateDir == 1)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[1, 3] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (rotate == -1 || rotate == 3)
                {
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 1] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[1, 2] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (Mathf.Abs(rotateDir) == 2)
                {
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;
                    blockData[3, 3] = (short)EChip.Exist;

                    blockData[1, 2] = (short)EChip.None;
                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                }

                break;
            case EBlockCategory.Stick:     // 棒(縦または横)
                // 移動制限
                if (rotate == 0 || Mathf.Abs(rotate) == 2)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMax;
                    }
                }
                else if (rotate == -3 || rotate == 1)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.Min)
                    {
                        newCoord.x = (short)EOffsetCoord.Min;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -1 || rotate == 3)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.Max)
                    {
                        newCoord.x = (short)EOffsetCoord.Max;
                    }
                }

                // 回転値同じなら回転処理は不要
                if (saveRotate == rotate)
                    break;
                // 回転による形状変化
                if (rotate == 0)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;
                    blockData[4, 2] = (short)EChip.Exist;

                    blockData[2, 0] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[2, 4] = (short)EChip.None;
                }
                else if (rotateDir == -3 || rotateDir == 1)
                {
                    blockData[2, 0] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;

                    blockData[0, 2] = (short)EChip.None;
                    blockData[1, 2] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                    blockData[4, 2] = (short)EChip.None;
                }
                else if (rotate == -1 || rotate == 3)
                {
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[2, 4] = (short)EChip.Exist;

                    blockData[0, 2] = (short)EChip.None;
                    blockData[1, 2] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                    blockData[4, 2] = (short)EChip.None;
                }
                else if (Mathf.Abs(rotateDir) == 2)
                {
                    blockData[0, 2] = (short)EChip.Exist;
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[2, 0] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[2, 4] = (short)EChip.None;
                }
                break;
            case EBlockCategory.Convex:    // 凸
                // 移動制限
                if (rotate == 0 || Mathf.Abs(rotate) == 2)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -3 || rotate == 1)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -1 || rotate == 3)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMax;
                    }
                }

                // 回転値同じなら回転処理は不要
                if (saveRotate == rotate)
                    break;
                // 回転による形状変化
                if (rotate == 0)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;

                    blockData[3, 2] = (short)EChip.None;
                }
                else if (rotateDir == -3 || rotateDir == 1)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[2, 1] = (short)EChip.None;
                }
                else if (rotate == -1 || rotate == 3)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[2, 3] = (short)EChip.None;
                }
                else if (Mathf.Abs(rotateDir) == 2)
                {
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 2] = (short)EChip.None;
                }

                break;
            case EBlockCategory.Z:         // Z
                // 移動制限
                if (rotate == 0 || Mathf.Abs(rotate) == 2)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -3 || rotate == 1)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMax;
                    }
                }
                else if (rotate == -1 || rotate == 3)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }

                // 回転値同じなら回転処理は不要
                if (saveRotate == rotate)
                    break;
                // 回転による形状変化
                if (rotate == 0)
                {
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;
                    blockData[3, 3] = (short)EChip.Exist;

                    blockData[1, 2] = (short)EChip.None;
                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                }
                else if (rotate == -3 || rotate == 1)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 1] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (rotate == -1 || rotate == 3)
                {
                    blockData[1, 3] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[1, 2] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (Mathf.Abs(rotate) == 2)
                {
                    blockData[1, 1] = (short)EChip.Exist;
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;

                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                }
                break;
            case EBlockCategory.InvZ:      // 逆Z
                // 移動制限
                if (rotate == 0 || Mathf.Abs(rotate) == 2)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -3 || rotate == 1)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMax;
                    }
                }
                else if (rotate == -1 || rotate == 3)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }

                // 回転値同じなら回転処理は不要
                if (saveRotate == rotate)
                    break;
                // 回転による形状変化
                if (rotate == 0)
                {
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 1] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[1, 2] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (rotate == -3 || rotate == 1)
                {
                    blockData[1, 1] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 2] = (short)EChip.None;
                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                }
                else if (rotate == -1 || rotate == 3)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 3] = (short)EChip.Exist;

                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                }
                else if (Mathf.Abs(rotate) == 2)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[1, 3] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                break;
            case EBlockCategory.L:         // L
                // 移動制限
                if (rotate == 0 || Mathf.Abs(rotate) == 2)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -3 || rotate == 1)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -1 || rotate == 3)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMax;
                    }
                }

                // 回転値同じなら回転処理は不要
                if (saveRotate == rotate)
                    break;
                // 回転による形状変化
                if (rotate == 0)
                {
                    blockData[1, 1] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;

                    blockData[1, 2] = (short)EChip.None;
                    blockData[1, 3] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                }
                else if (rotate == -3 || rotate == 1)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[1, 3] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (rotate == -1 || rotate == 3)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 1] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (Mathf.Abs(rotate) == 2)
                {
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 3] = (short)EChip.Exist;

                    blockData[1, 2] = (short)EChip.None;
                    blockData[1, 3] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                }
                break;
            case EBlockCategory.InvL:      // 逆L
                // 移動制限
                if (rotate == 0 || Mathf.Abs(rotate) == 2)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -3 || rotate == 1)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMax;
                    }
                }
                else if (rotate == -1 || rotate == 3)
                {
                    // 左限界値
                    if (newCoord.x <= (short)EOffsetCoord.MoreMin)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMin;
                    }
                    // 右限界値
                    else if (newCoord.x >= (short)EOffsetCoord.MoreMoreMax)
                    {
                        newCoord.x = (short)EOffsetCoord.MoreMoreMax;
                    }
                }

                // 回転値同じなら回転処理は不要
                if (saveRotate == rotate)
                    break;
                // 回転による形状変化
                if (rotate == 0)
                {
                    blockData[1, 3] = (short)EChip.Exist;
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[1, 2] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                else if (rotate == -3 || rotate == 1)
                {
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;
                    blockData[3, 3] = (short)EChip.Exist;

                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                }
                else if (rotate == -1 || rotate == 3)
                {
                    blockData[1, 1] = (short)EChip.Exist;
                    blockData[1, 2] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[3, 2] = (short)EChip.Exist;

                    blockData[1, 3] = (short)EChip.None;
                    blockData[2, 1] = (short)EChip.None;
                    blockData[2, 3] = (short)EChip.None;
                    blockData[3, 1] = (short)EChip.None;
                }
                else if (Mathf.Abs(rotate) == 2)
                {
                    blockData[2, 1] = (short)EChip.Exist;
                    blockData[2, 2] = (short)EChip.Exist;
                    blockData[2, 3] = (short)EChip.Exist;
                    blockData[3, 1] = (short)EChip.Exist;

                    blockData[1, 1] = (short)EChip.None;
                    blockData[1, 2] = (short)EChip.None;
                    blockData[3, 2] = (short)EChip.None;
                    blockData[3, 3] = (short)EChip.None;
                }
                break;
            default:
                break;
        }
        // 次からのスキップ用に回転情報を保存
        saveRotate = rotate;

        // 配置前にZangaiに触れているか/マップの下部にいるか確認
        // いるならblockDataの情報をZangaiに書き込み削除/初期位置に再生成
        bool moveStopFlag = false;

        // blockDataが一番下またはZangai[][] = Existに触れているかの処理
        // blockDataをMapに入れる前に行う
        for (short i = BLOCK_NUM_Y - 1; i >= 0; i--)
        {
            if (newCoord.y + i < 0 || newCoord.y + i >= MAPSIZE_Y)
                continue;

            for (short j = 0; j < BLOCK_NUM_X; j++)
            {
                // blockData[i][j]がEChip::Existでなければ/配列外なら処理を飛ばす
                if (blockData[i, j] != (short)EChip.Exist ||
                    newCoord.x + j < 0 || newCoord.x + j >= MAPSIZE_X)
                    continue;

                // 下:blockData内で一番下にあるブロック/blockData一番下列がブロックの場合を見る
                if (pushReturnFlag && ((i < (BLOCK_NUM_Y - 1) && blockData[i + 1, j] == (short)EChip.None) || (blockData[BLOCK_NUM_Y - 1, j] == (short)EChip.Exist)))
                {
                    // remainBlock = EChip::Existだったら
                    if (remainBlock[newCoord.y + i, newCoord.x + j] == (short)EChip.Exist)
                    {
                        // めり込んでいる分戻す
                        newCoord.y--;
                        // blockDataは止まる
                        moveStopFlag = true;
                    }

                    // 一つ先を見る
                    newCoord.y++;
                    // 一つ先が最下部だったら
                    if (newCoord.y + i >= MAPSIZE_Y)
                    {
                        // blockDataは止まる
                        moveStopFlag = true;
                    }
                    // 戻す
                    newCoord.y--;
                }
                // 左:jが最小ではないかつblockData内で一番左のブロック/blockData一番左列がブロックの場合を見る
                else if (pushLeftRightFlag && ((j > 0 && blockData[i, j - 1] == (short)EChip.None) || (blockData[i, 0] == (short)EChip.Exist)) &&
                    remainBlock[newCoord.y + i, newCoord.x + j] == (short)EChip.Exist)
                {
                    // 今いる座標にremainBlockがあるか
                    //if (remainBlock[newCoord.Y + i][newCoord.X + j] == EChip::Exist)
                    {
                        // めり込んでいる分戻す
                        newCoord.x++;
                        // blockDataはまだ止まらない
                        moveStopFlag = true;
                    }
                }
                // 右:jが最大ではないかつblockData内で一番右のブロック/blockData一番右列がブロックの場合を見る
                //else if (PushLeftRightFlag && ((j < (BLOCK_NUM_X - 1) && blockData[i][j + 1] == EChip::None) || (blockData[i][BLOCK_NUM_Y - 1] == EChip::Exist)) &&
                //	remainBlock[newCoord.Y + i][newCoord.X + j] == EChip::Exist)
                //{
                //	{
                //		// めり込んでいる分戻す
                //		newCoord.X--;
                //		// blockDataはまだ止まらない
                //		MoveStopFlag = FALSE;
                //	}
                //}
                else
                {
                    // 何もせずfor文を終了
                    break;
                }
            }
        }

        // blockDataをMapかZangaiに書き込む
        for (short i = 0; i < BLOCK_NUM_Y; i++)
        {
            for (short j = 0; j < BLOCK_NUM_X; j++)
            {
                // マップの配列外に出てしまうなら処理を飛ばす
                if (newCoord.y + i < 0 || newCoord.y + i >= MAPSIZE_Y ||
                    newCoord.x + j < 0 || newCoord.x + j >= MAPSIZE_X)
                    continue;

                if (!moveStopFlag)
                {
                    if (blockData[i, j] == (short)EChip.None)
                        continue;

                    // いなければMapに書き込み続行
                    map[newCoord.y + i, newCoord.x + j] = blockData[i, j];
                }
                else
                {
                    if (blockData[i, j] == (short)EChip.None)
                        continue;

                    remainBlock[newCoord.y + i, newCoord.x + j] = blockData[i, j];
                }
            }
        }

        short lastNumber = -1;
        short disappNum = 0;
        // Zangaiのi番目が一列そろっているかの確認
        for (short i = 0; i < MAPSIZE_Y; i++)
        {
            // 一列そろっているかの判定用カウント
            short alignCount = -1;
            for (short j = 0; j < MAPSIZE_X; j++)
            {
                // ブロックが置かれていなければ配置処理を飛ばす
                if (remainBlock[i, j] != (short)EChip.Exist)
                    continue;

                // ブロックの場合カウントを増やす
                alignCount++;

                // 一列がブロックでそろっていた場合
                if (alignCount == MAPSIZE_X - 1)
                {
                    // remainBlockの一列を削除する処理
                    while (alignCount > -1)
                    {
                        remainBlock[i, alignCount] = (short)EChip.None;
                        alignCount--;
                    }
                    lastNumber = i;
                    disappNum++;
                }
            }
        }

        // numが変わっている場合消えた分配列をずらす
        if (lastNumber != -1)
        {
            for (short i = lastNumber; i >= 0; i--)
            {
                for (short j = 0; j < MAPSIZE_X; j++)
                {
                    if (i - 1 < 0)
                        break;

                    saveRemainBlock[i, j] = remainBlock[i - disappNum, j];
                }
            }
            // 改めてremainBlockに配置
            for (short i = 0; i < lastNumber + 1; i++)
            {
                for (short j = 0; j < MAPSIZE_X; j++)
                {
                    remainBlock[i, j] = saveRemainBlock[i, j];
                }
            }
        }

        // remainBlockの再配置
        // 下からしゅっしゅっ
        for (short i = MAPSIZE_Y - 1; i >= 0; i--)
        {
            for (short j = 0; j < MAPSIZE_X; j++)
            {
                // ブロックが置かれていなければ配置処理を飛ばす
                if (remainBlock[i, j] != (short)EChip.Exist)
                    continue;

                map[i, j] = remainBlock[i, j];
            }
        }

        // blockDataが動けなければ
        if (moveStopFlag)
        {
            // 新しく生成
            EntryBlock(blockData);
        }

        // 入力関係を初期化
        pushRotateFlag = false;
        pushReturnFlag = false;
        pushLeftRightFlag = false;

        // 最終的なMapを描画
        Draw();
    }

    void Draw()
    {
        Vector2Int drawCoord = Vector2Int.zero;
        drawCoord.x = -5;
        drawCoord.y = 15;
        // マップの描画
        for (short i = 0; i < MAPSIZE_Y; i++)
        {
            for (short j = 0; j < MAPSIZE_X; j++)
            {
                switch (map[i, j])
                {
                    case (short)EChip.None:
                        // 何もなし
                        //if (drawBlock)
                        //{
                        //    Destroy(drawBlock);
                        //}
                        break;
                    case (short)EChip.Exist:
                        Instantiate(drawBlock, new Vector2(drawCoord.x + j, drawCoord.y - i), Quaternion.identity, mapBlockManage.transform);
                        break;
                    case (short)EChip.OutSide:
                        Instantiate(outsideDrawBlock, new Vector2(drawCoord.x + j, drawCoord.y - i), Quaternion.identity, mapBlockManage.transform);
                        break;
                }
            }
        }
    }
}
