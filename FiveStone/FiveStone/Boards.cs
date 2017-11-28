using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Reflection;
using System.IO;

namespace FiveStone
{
    public enum Color
    {
        Black,
        White
    }

    public enum Player
    {
        Human,
        Ai
    }
    public class Boards
    {
        public int[,] board = new int[15, 15];//棋盘中Ai的棋是1,人的棋是0,空位置是-1
        private Graphics mg;
        private Stones stone;
        private PC pc=new PC(false);
        private AI ai=new AI();

        private bool first = false;
        private bool winflag = false;

        private int persion_X = 0;//人类玩家上一次的落子点X
        private int persion_Y = 0;//人类玩家上一次的落子点Y

        public Color AiColor = Color.White;        //Ai执棋子的颜色
        public Color HumanColor = Color.Black;     //人类执棋子的颜色
        public Player CurrentTurn = Player.Human;  //现在轮到谁下棋

        public MyStack.MyStack st = new FiveStone.MyStack.MyStack();

        public Boards(Graphics g)
        {
            mg = g;
            stone = new Stones(mg);
            st.CreatStack();
            ai.AlphaBetaInit();
        }

        public void Start(bool mflag)
        {
            ClearBoards();
            winflag = false;
            if (!mflag)//AI执黑先行
            {
                AiColor = Color.Black;
                HumanColor = Color.White;
                CurrentTurn = Player.Ai;//轮到AI落子

                PCPut();
            }
            else//人类执黑先行
            {
                AiColor = Color.White;
                HumanColor = Color.Black;
                CurrentTurn = Player.Human;//轮到人类落子

            }
            DrawBoard();
        }

        public void ClearBoards()
        {
            for (int i = 0; i < 15; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    board[i, j] = -1;
                }
            }
        }

        public void DrawBoard()
        {
            Assembly myAssembly = Assembly.GetExecutingAssembly();
            Stream myStream = myAssembly.GetManifestResourceStream("FiveStone.board.png");
            Bitmap bt = new Bitmap(myStream);
            myStream.Close();
            mg.DrawImage(bt, 20, 20, bt.Width, bt.Height);

            for (int i = 0; i < 15; i++)
            {
                for (int j = 0; j < 15; j++)
                {
                    if (board[i, j] == 0)
                    {
                        stone.DrawStone(i, j, HumanColor);
                    }
                    if (board[i, j] == 1)
                    {
                        stone.DrawStone(i, j, AiColor);
                    }
                }
            }
        }
        /// <summary>
        /// 执行落子
        /// </summary>
        /// <param name="x">棋盘坐标</param>
        /// <param name="y">棋盘坐标</param>
        public void PutDown(int x, int y)
        {
            if (!winflag)
            {
                if (board[x, y] == -1)//待落子点是空位
                {
                    if (CurrentTurn == Player.Human)
                    {
                        //当前是人类玩家落子
                        stone.DrawStone(x, y, HumanColor);
                        board[x, y] = 0;
                    }
                    else
                    {
                        //当前是Ai落子回合
                        stone.DrawStone(x, y, AiColor);
                        board[x, y] = 1;
                    }


                    st.Insert((CurrentTurn == Player.Human?(HumanColor==Color.Black?"黑棋":"白棋"): (AiColor == Color.Black ? "黑棋" : "白棋")) +"X：" + x + " Y：" + y);

                    if (Rules.Winer(x, y, board) > 0)
                    {
                        switch (Rules.Winer(x, y, board))
                        {
                            case 1:
                                if (CurrentTurn==Player.Human)
                                {
                                    first = !first;
                                    winflag = true;
                                    System.Windows.Forms.MessageBox.Show("人类胜利");
                                }
                                else
                                {
                                    first = !first;
                                    winflag = true;
                                    System.Windows.Forms.MessageBox.Show("AI胜利");
                                }
                                break;
                            case 2:
                                first = !first;
                                winflag = true;
                                System.Windows.Forms.MessageBox.Show("平局");
                                break;
                        }
                    }
                }
            }
        }
        /// <summary>
        /// 人类玩家落子
        /// </summary>
        /// <param name="x">屏幕坐标X</param>
        /// <param name="y">屏幕坐标Y</param>
        public void PersonPut(int x, int y)
        {
            if (CurrentTurn == Player.Human) //现在是人类玩家落子回合
            {
                if (x < 680 && y < 680)
                {
                    int m = (int) (x / 42);
                    int n = (int) (y / 42);
                    if (m < 0)
                    {
                        m = 0;
                    }
                    if (n < 0)
                    {
                        n = 0;
                    }
                    if (m > 14)
                    {
                        m = 14;
                    }
                    if (n > 14)
                    {
                        n = 14;
                    }
                    persion_X = m;
                    persion_Y = n;
                    //if (!Rules.Exit(m, n, board))
                    {
                        PutDown(m, n);
                        CurrentTurn = Player.Ai;
                        PCPut();
                    }
                }
            }
        }
        /// <summary>
        /// Ai落子
        /// </summary>
        public void PCPut()
        {
            if (CurrentTurn == Player.Ai) //现在是Ai玩家落子回合
            {
                //源程序的AI
                //int m = 0, n = 0;
                //int err = 0;
                //do
                //{
                //    pc.Down(board);
                //    m = pc.X;
                //    n = pc.Y;
                //    err++;
                //    if (err > 100)
                //    {
                //        System.Windows.Forms.MessageBox.Show("发生了一些错误，棋局将重新开始");
                //        Start(true);
                //    }
                //}
                //while (Rules.Exit(m, n, board));
                //PutDown(m, n);

                //α-β剪枝AI
                ai.getComputerAction(persion_X, persion_Y, out int m, out int n);

                //if (!Rules.Exit(m, n, board))
                {
                    st.Insert("Ai计算结果:"+m+","+n);
                    PutDown(m, n);
                }
                CurrentTurn = Player.Human;
            }

        }
    }
}
