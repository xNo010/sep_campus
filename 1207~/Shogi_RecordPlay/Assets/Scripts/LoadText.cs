using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using UnityEngine;

public class LoadText : MonoBehaviour
{
    public struct POSITION
    {
        Int32 x;
        Int32 y;
    }

    public struct PLAYINFO
    {
        bool hand;

    }

    private string line;                // 一行を格納する文字列
    private List<string> lists;         // 可変リスト
    private string[] splitTest;         // 一行のコンマで分割される文字列配列
    private const Int32 splitNum = 8;   // 分割数

    void Start()
    {
        lists = new List<string>();

        string fileName = "Assets/Resources/GameRecord.txt";
        FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
        StreamReader sReader = new StreamReader(fs, Encoding.UTF8);

        if (sReader != null)
        {
            Debug.Log("あります");

            // 行単位で分割するためのループ
            Int32 count = 0;
            while (!sReader.EndOfStream)
            {
                // 配列に一行分を格納
                line = sReader.ReadLine();
                // 一行の情報を分割して配列に保存
                splitTest = line.Split(',');

                // 配列に保存したものをリストに送る
                lists.AddRange(splitTest);

                count++;
            }
            Debug.Log(lists.Count);
            Debug.Log(count + "回ループした");

            count = 0;
            PutPlayInfo(count);

            // ファイルを閉じる
            sReader.Close();
        }
    }

    void Update()
    {
        
    }

    void PutPlayInfo(Int32 count)
    {
        foreach (string str in lists)
        {
            Debug.Log(str);
            count++;
            if (count == 8)
            {
                break;
            }
        }
    }
}
