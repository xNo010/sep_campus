using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using UnityEngine;

public class MainGame : MonoBehaviour
{
    public enum EPIECE         // 駒の種類
    {
        none,
        own_Gold,
        own_Silver,
        own_Knight,
        own_Lance,
        own_Bishop,
        own_Rook,
        own_Pawn,
        own_King,
        enemy_Gold,
        enemy_Silver,
        enemy_Knight,
        enemy_Lance,
        enemy_Bishop,
        enemy_Rook,
        enemy_Pawn,
        enemy_King,
        maxPiece
    };

    public struct PLAYINFO
    {
        public bool                    hand;
        public GeneralElem.POSITION    selectPos;
        public GeneralElem.POSITION    movePos;
        public Int32                   piece;
        public bool                    isCallPromFunc;
        public bool                    isProm;
    }

    private string line;                    // 一行を格納する文字列
    private string[] splitStrings;          // 一行のコンマで分割される文字列配列
    private const Int32 SPLIT_NUM = 8;      // 分割数
    private List<PLAYINFO> playInfo;        // 棋譜情報を格納
    private GeneralElem.PIECEINFO piece;    // 駒情報

    public GameObject pieceParent;

    void Start()
    {
        // プレハブのロード
        LoadPrefabs();

        // 駒の配置
        PieceDeploy(pieceParent);

        playInfo = new List<PLAYINFO>();
        string fileName = "Assets/Resources/GameRecord.txt";
        // ファイル操作
        FileOperation(fileName);

        // 情報が入ったか確認
        AddingInfoCheck(playInfo);
    }

    void Update()
    {
        
    }

    // プレハブのロード
    public void LoadPrefabs()
    {
        piece.prefabs = new GameObject[(Int32)EPIECE.maxPiece];
        // 棋譜の駒IDと一致させる
        const Int32 ID_DIFF = 6;
        for (Int32 i = (Int32)EPIECE.none + 1; i < (Int32)EPIECE.maxPiece; i++)
        {
            switch ((EPIECE)i)
            {
                case EPIECE.own_Gold:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/Gold");
                    piece.iD = i;
                    break;
                case EPIECE.own_Silver:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/Silver");
                    piece.iD = i;
                    break;
                case EPIECE.own_Knight:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/Knight");
                    piece.iD = i;
                    break;
                case EPIECE.own_Lance:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/Lance");
                    piece.iD = i;
                    break;
                case EPIECE.own_Bishop:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/Bishop");
                    piece.iD = i;
                    break;
                case EPIECE.own_Rook:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/Rook");
                    piece.iD = i;
                    break;
                case EPIECE.own_Pawn:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/Pawn");
                    piece.iD = i;
                    break;
                case EPIECE.own_King:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/OwnPiece/King");
                    piece.iD = i;
                    break;
                case EPIECE.enemy_Gold:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/Gold");
                    piece.iD = i + ID_DIFF;
                    break;
                case EPIECE.enemy_Silver:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/Silver");
                    piece.iD = i + ID_DIFF;
                    break;
                case EPIECE.enemy_Knight:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/Knight");
                    piece.iD = i + ID_DIFF;
                    break;
                case EPIECE.enemy_Lance:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/Lance");
                    piece.iD = i + ID_DIFF;
                    break;
                case EPIECE.enemy_Bishop:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/Bishop");
                    piece.iD = i + ID_DIFF;
                    break;
                case EPIECE.enemy_Rook:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/Rook");
                    piece.iD = i + ID_DIFF;
                    break;
                case EPIECE.enemy_Pawn:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/Pawn");
                    piece.iD = i + ID_DIFF;
                    break;
                case EPIECE.enemy_King:
                    piece.prefabs[i] = (GameObject)Resources.Load("Prefabs/EnemyPiece/King");
                    piece.iD = i + ID_DIFF;
                    break;
                default:
                    break;
            }
        }
    }

    // 駒の配置
    public void PieceDeploy(GameObject pieceParent)
    {
        // 駒をマップに配置
        // 縦列・横列
        const Int32 VERT_NUM = 9;
        const Int32 HORI_NUM = 9;
        const Int32 CENTER = 10;
        const Int32 SIX_DIFF = 6;
        Int32 ownPieceInfo = (Int32)EPIECE.own_King;
        Int32 enemyPieceInfo = (Int32)EPIECE.enemy_King;
        // 自陣・敵陣に駒配置
        for (Int32 i = 0; i < VERT_NUM / 3; i++)
        {
            Int32 LeftShift = CENTER, RightShift = CENTER;
            switch (i)
            {
                case 0:
                    // 玉/王
                    Instantiate(piece.prefabs[ownPieceInfo], new Vector3(CENTER, -((VERT_NUM - i - 1) * 2 + 2), -0.1f), piece.prefabs[ownPieceInfo].transform.rotation, pieceParent.transform);
                    Instantiate(piece.prefabs[enemyPieceInfo], new Vector3(CENTER, -(i * 2 + 2), -0.1f), piece.prefabs[enemyPieceInfo].transform.rotation, pieceParent.transform);

                    ownPieceInfo = (Int32)EPIECE.none;
                    enemyPieceInfo = (Int32)EPIECE.enemy_Gold - 1;
                    // 金～香
                    while (LeftShift != 0 && RightShift != (HORI_NUM - 1) * 2 + 2)
                    {
                        LeftShift -= 2;
                        RightShift += 2;
                        ownPieceInfo++;
                        enemyPieceInfo++;
                        Instantiate(piece.prefabs[ownPieceInfo], new Vector3(LeftShift, -((VERT_NUM - i - 1) * 2 + 2), -0.1f), piece.prefabs[ownPieceInfo].transform.rotation, pieceParent.transform);
                        Instantiate(piece.prefabs[ownPieceInfo], new Vector3(RightShift, -((VERT_NUM - i - 1) * 2 + 2), -0.1f), piece.prefabs[ownPieceInfo].transform.rotation, pieceParent.transform);
                        Instantiate(piece.prefabs[enemyPieceInfo], new Vector3(LeftShift, -(i * 2 + 2), -0.1f), piece.prefabs[enemyPieceInfo].transform.rotation, pieceParent.transform);
                        Instantiate(piece.prefabs[enemyPieceInfo], new Vector3(RightShift, -(i * 2 + 2), -0.1f), piece.prefabs[enemyPieceInfo].transform.rotation, pieceParent.transform);
                    }
                    break;
                case 1:
                    LeftShift -= SIX_DIFF;
                    RightShift += SIX_DIFF;

                    ownPieceInfo++;
                    enemyPieceInfo++;
                    // 角
                    Instantiate(piece.prefabs[ownPieceInfo], new Vector3(LeftShift, -((VERT_NUM - i - 1) * 2 + 2), -0.1f), piece.prefabs[ownPieceInfo].transform.rotation, pieceParent.transform);
                    Instantiate(piece.prefabs[enemyPieceInfo], new Vector3(RightShift, -(i * 2 + 2), -0.1f), piece.prefabs[enemyPieceInfo].transform.rotation, pieceParent.transform);

                    ownPieceInfo++;
                    enemyPieceInfo++;
                    // 飛
                    Instantiate(piece.prefabs[ownPieceInfo], new Vector3(RightShift, -((VERT_NUM - i - 1) * 2 + 2), -0.1f), piece.prefabs[ownPieceInfo].transform.rotation, pieceParent.transform);
                    Instantiate(piece.prefabs[enemyPieceInfo], new Vector3(LeftShift, -(i * 2 + 2), -0.1f), piece.prefabs[enemyPieceInfo].transform.rotation, pieceParent.transform);
                    break;
                case 2:
                    // 歩
                    ownPieceInfo++;
                    enemyPieceInfo++;
                    for (Int32 j = 0; j < HORI_NUM; j++)
                    {
                        Instantiate(piece.prefabs[ownPieceInfo], new Vector3((j * 2 + 2), -((VERT_NUM - i - 1) * 2 + 2), -0.1f), piece.prefabs[ownPieceInfo].transform.rotation, pieceParent.transform);
                        Instantiate(piece.prefabs[enemyPieceInfo], new Vector3((j * 2 + 2), -(i * 2 + 2), -0.1f), piece.prefabs[enemyPieceInfo].transform.rotation, pieceParent.transform);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    public void FileOperation(string fileName)
    {
        FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
        StreamReader sReader = new StreamReader(fs, Encoding.UTF8);

        if (sReader != null)
        {
            // 行単位で分割するためのループ
            Int32 count = 0;
            while (!sReader.EndOfStream)
            {
                // 配列に一行分を格納
                line = sReader.ReadLine();

                // 一行の情報を分割して配列に保存
                splitStrings = line.Split(',');

                // 現在の分割情報を元に構造体に格納
                PutPlayInfo(splitStrings);
                count++;
            }
            Debug.Log(count + "回ループした");

            // ファイルを閉じる
            sReader.Close();
        }
    }

    // 情報を構造体に入れ込む
    public void PutPlayInfo(string[] splitNum)
    {
        // 先手か後手か
        const Int32 HAND_CHECK      = 0;
        // 選択駒座標(X)
        const Int32 S_POSX_CHECK    = 1;
        // 選択駒座標(Y)
        const Int32 S_POSY_CHECK    = 2;
        // 移動先座標(X)
        const Int32 M_POSX_CHECK    = 3;
        // 移動先座標(Y)
        const Int32 M_POSY_CHECK    = 4;
        // 駒の種類
        const Int32 PIECE_CHECK     = 5;
        // 成駒関数を呼んでいたかどうか
        const Int32 CALLFUNC_CHECK  = 6;
        // 成ったかどうか
        const Int32 PROM_CHECK      = 7;

        PLAYINFO tempInfo = new PLAYINFO();
        if (splitNum[HAND_CHECK] == "false")
        {
            tempInfo.hand = false;
        }
        else
        {
            tempInfo.hand = true;
        }
        // 選択駒座標
        tempInfo.selectPos.x = Int32.Parse(splitNum[S_POSX_CHECK]);
        tempInfo.selectPos.y = Int32.Parse(splitNum[S_POSY_CHECK]);
        // 移動先座標
        tempInfo.movePos.x = Int32.Parse(splitNum[M_POSX_CHECK]);
        tempInfo.movePos.y = Int32.Parse(splitNum[M_POSY_CHECK]);
        // 駒の種類
        tempInfo.piece = Int32.Parse(splitNum[PIECE_CHECK]);
        // 成駒関数を呼んでいたかどうか
        if (splitNum[CALLFUNC_CHECK] == "false")
        {
            tempInfo.isCallPromFunc = false;
            tempInfo.isProm = false;
        }
        else
        {
            tempInfo.isCallPromFunc = true;
            // 成ったかどうか
            if (splitNum[PROM_CHECK] == "false")
            {
                tempInfo.isProm = false;
            }
            else
            {
                tempInfo.isProm = true;
            }
        }

        // 構造体リストに格納
        playInfo.Add(tempInfo);
    }

    // 情報が入ったか確認
    public void AddingInfoCheck(List<PLAYINFO> lists)
    {
        foreach (PLAYINFO pi in lists)
        {
            Debug.Log(pi.hand.ToString());
            Debug.Log(pi.selectPos.x.ToString());
            Debug.Log(pi.selectPos.y.ToString());
            Debug.Log(pi.movePos.x.ToString());
            Debug.Log(pi.movePos.y.ToString());
            Debug.Log(pi.piece.ToString());
            Debug.Log(pi.isCallPromFunc.ToString());
            Debug.Log(pi.isProm.ToString());
        }
    }
}
