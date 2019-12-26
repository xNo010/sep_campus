using System.Collections;
using UnityEngine;

// サーバーから受け取る変数
[System.Serializable]
public class JSONData
{
    public int computer;    // comが選んだ番号
    public string message;  // 勝ち・引き分け・負け
}
