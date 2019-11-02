using UnityEngine;

public class Block : MonoBehaviour
{
    private Vector2 position;
    private const short movingValue = 1;
    private bool stopFlag;
    private short turnCount;
    private Transform[] child;

    private const short stopPosition = -8;

    void Start()
    {
        position = new Vector2(0.0f, 7.0f);

        child = new Transform[transform.childCount];
        for (short i = 0; i < transform.childCount; i++)
        {
            child[i] = transform.GetChild(i);
            Debug.Log(transform.childCount);
            Debug.Log(child[i]);
        }

        transform.position = position;
    }

    void Update()
    {
        turnCount++;

        if (!stopFlag && turnCount % 50 == 1)
        {
            position.y -= movingValue;
        }

        transform.position = position;

        // 子オブジェクトのどれかが地面に到達したら
        /*for (short i = 0; i < transform.childCount; i++)
        {
            if (child[i].transform.position.y + position.y <= stopPosition)
            {
                stopFlag = true;
            }
            Debug.Log(child[i].transform.position.y);
        }*/
        foreach (Transform child in transform)
        {
            //if ()
            Debug.Log(child.transform);
        }

        if (transform.position.y <= stopPosition)
        {
            position.y = stopPosition;
            transform.position = position;
            stopFlag = true;
        }
    }

    void OnCollisionEnter(Collision c)
    {
        stopFlag = true;
    }

    public Vector2 GetPosition()
    {
        return transform.position;
    }
    public void SetPosition(Vector2 Value)
    {
        transform.position = Value;
    }

    // 止まっているかどうか
    public bool IsStop()
    {
        return stopFlag;
    }
}
