using UnityEngine;

public class InputScript : MonoBehaviour
{
    private const short movingValue = 1;

    void Start()
    {

    }

    public void InputUpdate(Block block, Vector2 position)
    {
        // 入力情報
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            position.x -= movingValue;
        }
        if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            position.x += movingValue;
        }
        if (!block.IsStop() && Input.GetKeyDown(KeyCode.DownArrow))
        {
            position.y -= movingValue;
        }

        block.SetPosition(position);
    }

    public Transform GetTransform()
    {
        return this.transform;
    }
}
