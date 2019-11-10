using UnityEngine;

public class InputScript : MonoBehaviour
{
    void Start()
    {

    }

    public void InputUpdate(Vector2 position, short speed)
    {
        // 子に受け取った座標を入れ込む
        //transform.GetChild(0).transform.position = transform.position = position;

        // 入力情報
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            position.x -= speed;
            Debug.Log("左押されたy");
        }
        if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            position.x += speed;
            Debug.Log("右押されたy");
        }
        if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            position.y -= speed;
        }

        //transform.position = position;
    }

    public Transform GetTransform()
    {
        return this.transform;
    }
}
