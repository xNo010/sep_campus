using UnityEngine;
using System;

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

    public enum ERotationType
    {
        Standard,
        Right,
        Inverse,
        Left
    };

    public enum EOffsetCoord
    {
        MoreMoreMin = -7,
        MoreMin,
        Min,
        Max = 4,
        MoreMax,
        MoreMoreMax,
    };

    private string[] prefabsName = new string[(Int32)EBlockCategory.MaxNum]
    {
        "Prefabs/SquareBlock",
        "Prefabs/StickBlock",
        "Prefabs/ConvexBlock",
        "Prefabs/ZBlock",
        "Prefabs/InverseZBlock",
        "Prefabs/LBlock",
        "Prefabs/InverseLBlock"
    };

    private const Int32 BLOCK_NUM_X = 5;    // 5*5で作られるmoveBlock用
    private const Int32 BLOCK_NUM_Y = 5;    // 5*5で作られるmoveBlock用
    private const Int32 MAPSIZE_X = 10;     // マップの縦要素数
    private const Int32 MAPSIZE_Y = 25;     // マップの横要素数
    private Int32[,] map;                   // moveBlockが移動可能なマップ
    private Int32[,] remainBlock;           // moveBlockを送りマップに配置する
    private Int32[,] saveRemainBlock;       // RemainBlockを保存する

    private Vector2Int popCoord;            // 出現座標変数
    private Vector2Int newCoord;            // 更新座標変数
    private Int32 rotateDir;                // 回転情報変数
    private Int32 saveRotate;               // 回転情報の保存変数
    private const Int32 MOVING_VALUE = 1;   // 毎移動量
    private Int32[,] moveBlock;             // 動かすブロック
    private Int32[,] saveBlock;             // 動かすブロック情報を保存
    private GameObject drawBlock;
    private GameObject outsideDrawBlock;

    [SerializeField]
    public GameObject mapBlockManage;
    [SerializeField]
    public InputScript inputScript;         // 入力情報スクリプト

    private BlockManage blockManage;        // 更新された座標をセットする為のコンポーネント

    private const short MAX_BLOCK_TABLE = 30;
    private EBlockCategory entryNumber;     // ランダムに生成される登録番号
    private short[] entryBlockTable;        // 表示ブロックテーブル

    //private bool leftRotateFlag;    // 回転キーを押したかどうか
    //private bool leftMoveFlag;      // 左右キーを押したかどうか
    //private bool rightMoveFlag;     // 左右キーを押したかどうか
    //private bool pushDownFlag;      // 下キーを押したかどうか
    //private bool pushReturnFlag;    // エンターキーを押したかどうか
    //private bool endFlag;           // 終わるかどうか
    private bool createFlag;

    private float turnCount;        // 回るタイミング

    void Start()
    {
        Initialize();

        // 指定座標に、キーを対応させたブロックを生成
    }

    void Update()
    {
        //inputScript.InputUpdate(MOVING_VALUE);

        turnCount += Time.deltaTime;
        if (turnCount >= 3.0f)
        {
            turnCount = 0.0f;
            newCoord.y += MOVING_VALUE;

            //Debug.Log(newCoord);

            Execute(/*moveBlock, newCoord, rotateDir, endFlag*/);
        }
    }

    void Initialize()
    {
        // アセットの読み込み
        drawBlock = (GameObject)Resources.Load("Prefabs/OneBlock");
        outsideDrawBlock = (GameObject)Resources.Load("Prefabs/RedOneBlock");

        // 配列の初期化
        map = new Int32[MAPSIZE_Y, MAPSIZE_X];
        remainBlock = new Int32[MAPSIZE_Y, MAPSIZE_X];
        saveRemainBlock = new Int32[MAPSIZE_Y, MAPSIZE_X];
        moveBlock = new Int32[BLOCK_NUM_Y, BLOCK_NUM_X];
        saveBlock = new Int32[BLOCK_NUM_Y, BLOCK_NUM_X];

        // 変数を初期化
        popCoord = Vector2Int.zero;
        newCoord = Vector2Int.zero;
        rotateDir = (Int32)ERotationType.Standard;
        saveRotate = rotateDir;
        entryNumber = EBlockCategory.Square;
        turnCount = 0.0f;

        //leftRotateFlag = false;
        //leftMoveFlag = false;
        //rightMoveFlag = false;
        //pushDownFlag = false;
        //pushReturnFlag = false;
        //endFlag = false;
        createFlag = true;

        // mapのカメラ範囲外のところを設定
        for (Int32 i = 0; i < MAPSIZE_Y - 20; i++)
        {
            for (Int32 j = 0; j < MAPSIZE_X; j++)
            {
                map[i, j] = (Int32)EChip.OutSide;
            }
        }

        entryBlockTable = new short[MAX_BLOCK_TABLE];

        // 最初に何らかの形のブロックを作る
        EntryBlock();

        // 初期マップ描画
        Draw();
    }

    void EntryBlock()
    {
        // 乱数で形を決定
        //for (Int32 i = 0; i < MAX_BLOCK_TABLE; i++)
        //{
        //    entryBlockTable[i] = (short)UnityEngine.Random.Range((Int32)EBlockCategory.Square, (Int32)EBlockCategory.MaxNum);
        //}
        //entryNumber = (EBlockCategory)UnityEngine.Random.Range((Int32)EBlockCategory.Square, (Int32)EBlockCategory.MaxNum);

        // デバッグ用
        entryNumber = EBlockCategory.Convex;

        Debug.Log(entryNumber);

        // ブロック生成
        // [2][2]を中心とする
        switch (entryNumber)
        {
            case EBlockCategory.Square:    // 四角
                saveBlock[2, 2] = (Int32)EChip.Exist;
                saveBlock[2, 3] = (Int32)EChip.Exist;
                saveBlock[3, 2] = (Int32)EChip.Exist;
                saveBlock[3, 3] = (Int32)EChip.Exist;
                break;
            case EBlockCategory.Stick:     // 棒(縦または横)
                for (Int32 i = 0; i <= 3; i++)
                {
                    saveBlock[i, 2] = (Int32)EChip.Exist;
                }
                break;
            case EBlockCategory.Convex:    // 凸
                saveBlock[2, 1] = (Int32)EChip.Exist;
                saveBlock[2, 2] = (Int32)EChip.Exist;
                saveBlock[2, 3] = (Int32)EChip.Exist;
                saveBlock[1, 2] = (Int32)EChip.Exist;
                break;
            case EBlockCategory.Z:         // Z
                saveBlock[1, 1] = (Int32)EChip.Exist;
                saveBlock[1, 2] = (Int32)EChip.Exist;
                saveBlock[2, 1] = (Int32)EChip.Exist;
                saveBlock[2, 2] = (Int32)EChip.Exist;
                break;
            case EBlockCategory.InvZ:      // 逆Z
                saveBlock[1, 2] = (Int32)EChip.Exist;
                saveBlock[1, 3] = (Int32)EChip.Exist;
                saveBlock[2, 1] = (Int32)EChip.Exist;
                saveBlock[2, 2] = (Int32)EChip.Exist;
                break;
            case EBlockCategory.L:         // L
                saveBlock[2, 1] = (Int32)EChip.Exist;
                saveBlock[2, 2] = (Int32)EChip.Exist;
                saveBlock[2, 3] = (Int32)EChip.Exist;
                saveBlock[3, 3] = (Int32)EChip.Exist;
                break;
            case EBlockCategory.InvL:      // 逆L
                saveBlock[2, 1] = (Int32)EChip.Exist;
                saveBlock[2, 2] = (Int32)EChip.Exist;
                saveBlock[2, 3] = (Int32)EChip.Exist;
                saveBlock[3, 3] = (Int32)EChip.Exist;
                break;
            default:
                break;
        }

        //moveBlock = saveBlock;

        // 回転情報初期化
        rotateDir = 0;
        saveRotate = rotateDir;
        // 初期位置の決定
        popCoord.x = -2;
        popCoord.y = 9;
        newCoord = popCoord;

        for (Int32 i = 0; i < BLOCK_NUM_Y; i++)
        {
            for (Int32 j = 0; j < BLOCK_NUM_X; j++)
            {
                if (saveBlock[i,j] != (Int32)EChip.Exist)
                    continue;

                Instantiate(drawBlock, new Vector2(popCoord.x + j, popCoord.y - i), Quaternion.identity, inputScript.transform);
            }
        }

        // マップに配置
        for (Int32 i = 0; i < BLOCK_NUM_Y; i++)
        {
            for (Int32 j = 0; j < BLOCK_NUM_X; j++)
            {
                if (saveBlock[i, j] != (Int32)EChip.Exist || 
                    popCoord.y - i < 0 || popCoord.y - i > MAPSIZE_Y || 
                    popCoord.x + j < 0 || popCoord.x + j > MAPSIZE_X)
                    continue;

                map[popCoord.y - i, popCoord.x + j] = saveBlock[i, j];
            }
        }
    }

    void Execute()
    {
        // --- マップの更新 ---
        // Mapをまっさらな状態に
        for (Int32 i = 0; i < MAPSIZE_Y; i++)
        {
            for (Int32 j = 0; j < MAPSIZE_X; j++)
            {
                if (i < 5)
                {
                    map[i, j] = (Int32)EChip.OutSide;
                }
                else
                {
                    map[i, j] = (Int32)EChip.None;
                }
            }
        }

        foreach (Transform child in mapBlockManage.transform)
        {
            Destroy(child.gameObject);
        }
        createFlag = false;

        /*bool moveStopFlag = false;

        // blockDataをMapかZangaiに書き込む
        for (Int32 i = 0; i < BLOCK_NUM_Y; i++)
        {
            for (Int32 j = 0; j < BLOCK_NUM_X; j++)
            {
                // マップの配列外に出てしまうなら処理を飛ばす
                if (newCoord.y + i < 0 || newCoord.y + i >= MAPSIZE_Y ||
                    newCoord.x + j < 0 || newCoord.x + j >= MAPSIZE_X)
                    continue;

                if (!moveStopFlag)
                {
                    if (moveBlock[i, j] == (Int32)EChip.None)
                        continue;

                    // いなければMapに書き込み続行
                    map[newCoord.y + i, newCoord.x + j] = moveBlock[i, j];
                }
                else
                {
                    if (moveBlock[i, j] == (Int32)EChip.None)
                        continue;

                    remainBlock[newCoord.y + i, newCoord.x + j] = moveBlock[i, j];
                }
            }
        }

        Int32 lastNumber = -1;
        Int32 disappNum = 0;
        // Zangaiのi番目が一列そろっているかの確認
        for (Int32 i = 0; i < MAPSIZE_Y; i++)
        {
            // 一列そろっているかの判定用カウント
            Int32 alignCount = -1;
            for (Int32 j = 0; j < MAPSIZE_X; j++)
            {
                // ブロックが置かれていなければ配置処理を飛ばす
                if (remainBlock[i, j] != (Int32)EChip.Exist)
                    continue;

                // ブロックの場合カウントを増やす
                alignCount++;

                // 一列がブロックでそろっていた場合
                if (alignCount == MAPSIZE_X - 1)
                {
                    // remainBlockの一列を削除する処理
                    while (alignCount > -1)
                    {
                        remainBlock[i, alignCount] = (Int32)EChip.None;
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
            for (Int32 i = lastNumber; i >= 0; i--)
            {
                for (Int32 j = 0; j < MAPSIZE_X; j++)
                {
                    if (i - 1 < 0)
                        break;

                    saveRemainBlock[i, j] = remainBlock[i - disappNum, j];
                }
            }
            // 改めてremainBlockに配置
            for (Int32 i = 0; i < lastNumber + 1; i++)
            {
                for (Int32 j = 0; j < MAPSIZE_X; j++)
                {
                    remainBlock[i, j] = saveRemainBlock[i, j];
                }
            }
        }

        // remainBlockの再配置
        // 下からしゅっしゅっ
        for (Int32 i = MAPSIZE_Y - 1; i >= 0; i--)
        {
            for (Int32 j = 0; j < MAPSIZE_X; j++)
            {
                // ブロックが置かれていなければ配置処理を飛ばす
                if (remainBlock[i, j] != (Int32)EChip.Exist)
                    continue;

                map[i, j] = remainBlock[i, j];
            }
        }

        // blockDataが動けなければ
        if (moveStopFlag)
        {
            // 新しく生成
            EntryBlock();
        }

        // 入力関係を初期化
        leftRotateFlag = false;
        pushReturnFlag = false;
        leftMoveFlag = false;
        rightMoveFlag = false;
        */
        // 最終的なMapを描画
        Draw();
    }

    void Draw()
    {
        Vector2Int drawCoord = Vector2Int.zero;
        drawCoord.x = -5;
        drawCoord.y = 15;
        // マップの描画
        for (Int32 i = 0; i < MAPSIZE_Y; i++)
        {
            for (Int32 j = 0; j < MAPSIZE_X; j++)
            {
                switch (map[i, j])
                {
                    case (Int32)EChip.None:
                        // 何もなし
                        //if (drawBlock)
                        //{
                        //    Destroy(drawBlock);
                        //}
                        break;
                    case (Int32)EChip.Exist:
                        //Instantiate(drawBlock, new Vector2(drawCoord.x + j, drawCoord.y - i), Quaternion.identity, mapBlockManage.transform);
                        break;
                    case (Int32)EChip.OutSide:
                        if (createFlag)
                        {
                            Instantiate(outsideDrawBlock, new Vector2(drawCoord.x + j, drawCoord.y - i), Quaternion.identity, mapBlockManage.transform);
                        }
                        break;
                }
            }
        }
    }
}
