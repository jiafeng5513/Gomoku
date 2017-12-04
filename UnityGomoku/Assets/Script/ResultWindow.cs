using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class ResultWindow : MonoBehaviour
{

    // 重玩按钮
    public Button ReplayButton;
    // 退出按钮
    public Button offButton;
    // 提示文字
    public Text Message;

    // 主循环句柄
    public MainLoop mainLoop;

    void Start()
    {
        ReplayButton.onClick.AddListener(() =>
        {
            gameObject.SetActive(false);
            mainLoop.Restart();
        });
        offButton.onClick.AddListener(() =>
        {
            gameObject.SetActive(false);
            Application.Quit();
        });
    }

    public void Show(ChessType wintype)
    {
        switch (wintype)
        {
            case ChessType.Black:
                {
                    Message.text = string.Format("恭喜, 你战胜了电脑!");
                }
                break;
            case ChessType.White:
                {
                    Message.text = string.Format("你被电脑击败了!");
                }
                break;
        }
    }
}
