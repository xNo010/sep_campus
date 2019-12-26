using System;
using System.Collections;
using UnityEngine;
using UnityEngine.Networking;

public class WebConnect : MonoBehaviour
{
    public enum EJANKEN_CATE
    {
        Rock,
        Scissor,
        Paper
    }

    private const string URL = "http://ec2-18-176-58-134.ap-northeast-1.compute.amazonaws.com/janken.php";
    private Int32 eHand;
    private JSONData json;  // 受け取るjsonデータ
    WWWForm form;           // 自分の選択した手を送る
    private WinOrLose wol;  // 勝ち負けを設定する

    void Start()
    {
        form = new WWWForm();
        wol = GameObject.Find("WinorLose").GetComponent<WinOrLose>();
    }

    // 自分の手を選択
    public void HandSelect_Rock()
    {
        eHand = (int)EJANKEN_CATE.Rock;
        // formに手の情報を追加
        form.AddField("you", eHand.ToString());

        // コルーチンを開始して、サーバーの選択を要求
        StartCoroutine(Connect(URL, form));
    }
    // 自分の手を選択
    public void HandSelect_Scissor()
    {
        eHand = (int)EJANKEN_CATE.Scissor;
        // formに手の情報を追加
        form.AddField("you", eHand.ToString());

        // コルーチンを開始して、サーバーの選択を要求
        StartCoroutine(Connect(URL, form));
    }
    // 自分の手を選択
    public void HandSelect_Paper()
    {
        eHand = (int)EJANKEN_CATE.Paper;
        // formに手の情報を追加
        form.AddField("you", eHand.ToString());

        // コルーチンを開始して、サーバーの選択を要求
        StartCoroutine(Connect(URL, form));
    }

    // サーバーに接続
    IEnumerator Connect(string url, WWWForm form)
    {
        //Debug.Log(eHand);

        // formをurlに追加(Post)
        UnityWebRequest webRequest = UnityWebRequest.Post(url, form);

        yield return webRequest.SendWebRequest();

        // エラーチェック
        if (webRequest.isNetworkError)
        {
            //通信失敗
            Debug.Log(webRequest.error);
        }
        else
        {
            //通信成功
            //Debug.Log(webRequest.downloadHandler.text);
            //Debug.Log(webRequest.url);

            // 情報を受け取る
            json = JsonUtility.FromJson<JSONData>(webRequest.downloadHandler.text);

            //Debug.Log(json.computer);
            //Debug.Log(json.message);

            // Unity上で勝敗を更新
            json.message = wol.WinOrLoseCheck(eHand, json.computer);

            // 結果のテキストを表示
            wol.TextUpdate(json.message);

        }
    }
}
