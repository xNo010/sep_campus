using UnityEngine;
using System;

public class InputScript : MonoBehaviour
{
    void Start()
    {

    }

    public void InputUpdate(Int32 movingValue)
    {
        // 子に受け取った座標を入れ込む
        //transform.GetChild(0).transform.position = transform.position = position;

        Vector2 position = new Vector2();

        // 入力情報
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            position.x -= movingValue;
            Debug.Log("左押されたy");
        }
        if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            position.x += movingValue;
            Debug.Log("右押されたy");
        }
        if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            position.y -= movingValue;
        }
    }

    public Transform GetTransform()
    {
        return this.transform;
    }
}
