using System;
using System.Collections.Generic;
using UnityEngine;

public class GeneralElem : MonoBehaviour
{
    public struct POSITION
    {
        public Int32 x;
        public Int32 y;
    }

    public struct PIECEINFO
    {
        public Int32 iD;
        public GameObject[] prefabs;
    }
}
