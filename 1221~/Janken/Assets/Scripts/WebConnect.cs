using System;
using System.Collections;
using UnityEngine;
using UnityEngine.Networking;
using Common;

public class WebConnect : MonoBehaviour
{
    private const string URL = "http://ec2-18-176-58-134.ap-northeast-1.compute.amazonaws.com/janken.php";
    private EJANKEN_CATE eHand;
    private JSONData json;  // 受け取るjsonデータ
    WWWForm form;           // 自分の選択した手を送る
    
    [SerializeField]
    private WinOrLose wol = null;  // 勝ち負けを設定する

    void Start()
    {
        form = new WWWForm();
        Debug.Log("デバッグ実行");
    }

    // 自分の手を選択
    public void HandSelect_Rock()
    {
        eHand = EJANKEN_CATE.Rock;
        // formに手の情報を追加
        form.AddField("you", eHand.ToString());

        // コルーチンを開始して、サーバーの選択を要求
        StartCoroutine(Connect(URL, form));
    }
    // 自分の手を選択
    public void HandSelect_Scissor()
    {
        eHand = EJANKEN_CATE.Scissor;
        // formに手の情報を追加
        form.AddField("you", eHand.ToString());

        // コルーチンを開始して、サーバーの選択を要求
        StartCoroutine(Connect(URL, form));
    }
    // 自分の手を選択
    public void HandSelect_Paper()
    {
        eHand = EJANKEN_CATE.Paper;
        // formに手の情報を追加
        form.AddField("you", eHand.ToString());

        // コルーチンを開始して、サーバーの選択を要求
        StartCoroutine(Connect(URL, form));
    }

    // サーバーに接続
    IEnumerator Connect(string url, WWWForm form)
    {
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
