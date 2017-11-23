using UnityEngine;
using System.Collections;
using UnityEngine;
using UnityEngine.UI;
public class Cross : MonoBehaviour {


    // 位置
    public int GridX;
    public int GridY;
	// Use this for initialization
    public MainLoop mainLoop;
	void Start () {
        GetComponent<Button>().onClick.AddListener(() =>
        {
            mainLoop.OnClick(this);
        });
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
