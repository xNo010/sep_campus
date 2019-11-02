using UnityEngine;

public class BoardScript : MonoBehaviour
{
    private const short Board_Size_X = 16;
    private const short Board_Size_Y = 16;

    //なんのオブジェクトを使うかの設定
    [SerializeField] GameObject blockPrefad = null;
    
    private GameObject[,] boardBlock = new GameObject[Board_Size_Y, Board_Size_X];
    
    void Start()
    {
        //入れたオブジェクトを生成
        
        for (int i = 0; i < Board_Size_Y; i++)
        {
            for (int j = 0; j < Board_Size_Y; j++)
            {
                GameObject Object = Instantiate(blockPrefad);
                Object.transform.position = new Vector2(j, i);
                boardBlock[i, j] = Object;
            }
        }
    }
    
    void Update()
    {

    }
}
