using System;
using System.Collections;
using UnityEngine;
using UnityEngine.UI;

public class WinOrLose : MonoBehaviour
{
    public Text winOrLoseText;
    public Text comHand;

    // 勝敗の判定
    public string WinOrLoseCheck(Int32 mySelectNum, Int32 comSelectNum)
    {
        // 相手の手は何か
        switch ((WebConnect.EJANKEN_CATE)comSelectNum)
        {
            case WebConnect.EJANKEN_CATE.Rock:
                comHand.text = "com:グー";
                break;
            case WebConnect.EJANKEN_CATE.Scissor:
                comHand.text = "com:チョキ";
                break;
            case WebConnect.EJANKEN_CATE.Paper:
                comHand.text = "com:パー";
                break;
            default:
                break;
        }

        string str;
        if (mySelectNum == comSelectNum)
        {
            str = "Result:引き分け";
        }
        else if ((mySelectNum + 1) % 3 == comSelectNum)
        {
            str = "Result:勝ち";
        }
        else
        {
            str = "Result:負け";
        }

        return str;
    }

    // テキストの更新
    public void TextUpdate(string text)
    {
        winOrLoseText.text = text;
    }
}
