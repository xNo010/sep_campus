using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Background : MonoBehaviour
{
    void Start()
    {
        // 茶色に変更.
        GetComponent<Renderer>().material.color = new Color32(204, 153, 51, 1);
    }
}
