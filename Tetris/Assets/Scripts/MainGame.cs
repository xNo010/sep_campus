using UnityEngine;

enum ESelectBlock   // 選ばれるブロックの種類
{
    None,
    Deko,
    Horizon,
    Square,
    LType,
    InvLType,
    ZigZag,
    InvZigZag,
    MaxNum
}

public class MainGame : MonoBehaviour
{
    private const short MAPSIZE_X = 12; // マップの縦要素数
    private const short MAPSIZE_Y = 16; // マップの横要素数
    private char[,] Map;                // Blockが移動可能なマップ

    private Vector2 popCoord;           // 出現座標
    private InputScript inputScript;    // 入力情報スクリプト
    private GameObject block;           // 生成されるblockオブジェクト
    private Block blockScript;          // 更新された座標をセットする為のコンポーネント
    private ESelectBlock entryNumber;   // ランダムに生成される登録番号

    void Start()
    {
        inputScript = GameObject.Find("InputParent").GetComponent<InputScript>();
        // Map = new char[MAPSIZE_X, MAPSIZE_Y];

        // 番号によって読み込むプレハブが変わる
        entryNumber = (ESelectBlock)Random.Range((int)ESelectBlock.Deko, (int)ESelectBlock.MaxNum);
        switch (entryNumber)
        {
            case ESelectBlock.Deko:
                block = (GameObject)Resources.Load("Prefabs/DekoBlock");
                break;
            case ESelectBlock.Horizon:
                block = (GameObject)Resources.Load("Prefabs/HorizonBlock");
                break;
            case ESelectBlock.Square:
                block = (GameObject)Resources.Load("Prefabs/SquareBlock");
                break;
            case ESelectBlock.LType:
                block = (GameObject)Resources.Load("Prefabs/LBlock");
                break;
            case ESelectBlock.InvLType:
                block = (GameObject)Resources.Load("Prefabs/InverseLBlock");
                break;
            case ESelectBlock.ZigZag:
                block = (GameObject)Resources.Load("Prefabs/ZigZagBlock");
                break;
            case ESelectBlock.InvZigZag:
                block = (GameObject)Resources.Load("Prefabs/InverseZigZagBlock");
                break;
            default:
                break;
        }

        // 指定座標に、キーに対応したブロックを生成
        popCoord = new Vector2(0.0f, 6.0f);
        Instantiate(block, popCoord, inputScript.transform.rotation/*, inputScript.GetTransform()*/);

        blockScript = block.GetComponent<Block>();
        blockScript.SetPosition(popCoord);
    }

    void Update()
    {
        inputScript.InputUpdate(blockScript, popCoord);
        //Debug.Log(blockScript.GetPosition());
    }
}
