using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace GoBangProject.structs
{

    /// <summary>
    /// 方向枚举
    /// </summary>
    public enum Direction
    {
        UP,
        UpLeft,
        Left,
        DownLeft,
        Down,
        DownRight,
        Right,
        UpRight
    }
    public class ChessRule
    {
        /// <summary>
        /// 按照方向,探测棋子在某一方向上棋子相同的个数
        /// </summary>
        /// <param name="direct"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        public static int CheckDirectOnChess(Point point,Direction direct,ChessType type)
        {
            //缓存当前坐标的引用
            ChessPoint tempPoint = Chess.Find(Chess.Find(point).DirectionPoint(direct));
            //循环计数
            int count = 0;
            //寻找
            while (true)
            {
                if (tempPoint != null && tempPoint.Type == type)
                {
                    count++;
                    tempPoint = Chess.Find(tempPoint.DirectionPoint(direct));
                }
                else
                    break;
            }
            return count;
            
        }

        /// <summary>
        /// 对各个方向进行判断
        /// </summary>
        /// <returns>是否赢得比赛</returns>
        public static Boolean CheckWin()
        {
            //如果为黑方,判断禁手
            if(Conf.CurrentTurn == ChessType.BLACK)
            {
                //判断是否为4-4禁手
                if (CheckAbort4_4())
                {
                    AbortWinShow("四-四");
                    return false;
                }

                //判断是否为3-3禁手
                if (CheckAbort3_3())
                {
                    AbortWinShow("三-三");
                    return false;
                }

                //判断是否为长连禁手
                if (CheckAbortLong())
                {
                    AbortWinShow("长连");
                    return false;
                }
            }

            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.Down, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UP, Conf.CurrentTurn) + 1 == 5)
                return true;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.DownLeft, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UpRight, Conf.CurrentTurn) + 1 == 5)
                return true;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.DownRight, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UpLeft, Conf.CurrentTurn) + 1 == 5)
                return true;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.Left, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.Right, Conf.CurrentTurn) + 1 == 5)
                return true;

            return false;
        }

        /// <summary>
        /// 交换下棋方
        /// </summary>
        public static void ChangeType()
        {
            if (Conf.CurrentTurn == ChessType.BLACK)
                //改变下棋方
                Conf.CurrentTurn = ChessType.WHITE;
            else
                //改变下棋方
                Conf.CurrentTurn = ChessType.BLACK;

            //添加历史缓存
            Conf.globleRoute.Add(new HistoryRecord(Conf.LastPoint));

            //显示历史记录
            Form1.formInstance.AddRecordToList(Conf.globleRoute[Conf.globleRoute.Count - 1].Text);
        }

        /// <summary>
        /// 赢得比赛的显示
        /// </summary>
        public static void WinShow()
        {
            MessageBox.Show(Conf.CurrentTurn + "赢得比赛","恭喜!");
            Form1.formInstance.AbleAllCompents(false);
        }

        /// <summary>
        /// 比赛平局的显示
        /// </summary>
        public static void WinPeace()
        {
            MessageBox.Show("两方平局", "你俩够彪的!");
            Form1.formInstance.AbleAllCompents(false);
        }


        /// <summary>
        /// 通过禁手赢得比赛的显示
        /// </summary>
        public static void AbortWinShow(String text)
        {
            ChessType temp = ChessType.NULL;
            if (Conf.CurrentTurn == ChessType.BLACK)
                temp = ChessType.WHITE;
            else
                temp = ChessType.BLACK;
            MessageBox.Show(temp + "通过对方的 "+text+"禁手 赢得比赛", "恭喜!");

            Form1.formInstance.AbleAllCompents(false);
        }

        /// <summary>
        /// 悔棋规则
        /// </summary>
        public static void RegretPoint(int index)
        {
            for (int i = Conf.globleRoute.Count-1; i >=index; i--)
            {
                //设置选择状态
                Conf.globleRoute[i].Point.HistorySelect = false;
                //设置棋子类型
                Conf.globleRoute[i].Point.Type = ChessType.NULL;
                //删除棋子
                Conf.globleRoute.Remove(Conf.globleRoute[i]);
            }
        }

        #region 禁手规则

        /// <summary>
        /// 判断某一方向是否为活N
        /// </summary>
        /// <param name="direct"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        public static int checkDirectOnChessForActive(Point point,Direction direct, ChessType type)
        {

            //缓存当前坐标的引用
            ChessPoint tempPoint = Chess.Find(Chess.Find(point).DirectionPoint(direct));
            //循环计数
            int count = 0;
            //寻找
            while (true)
            {
                if (tempPoint != null && tempPoint.Type == type)
                {
                    count++;
                    tempPoint = Chess.Find(tempPoint.DirectionPoint(direct));
                }
                else
                {
                    if (tempPoint != null && tempPoint.Type == ChessType.NULL)
                        return count;
                    else
                        return -125;
                }
            }
        }

        /// <summary>
        /// 3-3禁手
        /// </summary>
        /// <returns></returns>
        public static Boolean CheckAbort3_3()
        {
            int assertCount = 0;
            if (checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.Down, Conf.CurrentTurn) + checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.UP, Conf.CurrentTurn) + 1 == 3)
                assertCount++;
            if (checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.DownLeft, Conf.CurrentTurn) + checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.UpRight, Conf.CurrentTurn) + 1 == 3)
                assertCount++;
            if (checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.DownRight, Conf.CurrentTurn) + checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.UpLeft, Conf.CurrentTurn) + 1 == 3)
                assertCount++;
            if (checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.Left, Conf.CurrentTurn) + checkDirectOnChessForActive(Conf.LastPoint.Point, Direction.Right, Conf.CurrentTurn) + 1 == 3)
                assertCount++;

            return assertCount >= 2;
        }

        /// <summary>
        /// 4-4禁手
        /// </summary>
        /// <returns></returns>
        public static Boolean CheckAbort4_4()
        {
            int assertCount = 0;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.Down, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UP, Conf.CurrentTurn) + 1 == 4)
                assertCount++;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.DownLeft, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UpRight, Conf.CurrentTurn) + 1 == 4)
                assertCount++;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.DownRight, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UpLeft, Conf.CurrentTurn) + 1 == 4)
                assertCount++;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.Left, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.Right, Conf.CurrentTurn) + 1 == 4)
                assertCount++;

            return assertCount >= 2;
        }

        /// <summary>
        /// 长连禁手
        /// </summary>
        /// <returns></returns>
        public static Boolean CheckAbortLong()
        {
            int assertCount = 0;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.Down, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UP, Conf.CurrentTurn) + 1 > 5)
                assertCount++;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.DownLeft, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UpRight, Conf.CurrentTurn) + 1 > 5)
                assertCount++;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.DownRight, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.UpLeft, Conf.CurrentTurn) + 1 > 5)
                assertCount++;
            if (CheckDirectOnChess(Conf.LastPoint.Point, Direction.Left, Conf.CurrentTurn) + CheckDirectOnChess(Conf.LastPoint.Point, Direction.Right, Conf.CurrentTurn) + 1 > 5)
                assertCount++;

            return assertCount >= 2;
        }
        #endregion
    }
}
