using System;
using System.Collections.Generic;
using System.Text;
using GoBangProject.structs;
using System.Drawing;

namespace GoBangProject.GeneticAlgorithm
{
    /// <summary>
    /// 人工智能,单子威力判断规则
    /// </summary>
    public class AIRule
    {

        /// <summary>
        /// 判断指定方向以内,是否有子存在
        /// </summary>
        /// <param name="direct"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        public static Boolean CheckDirectOnChess(Point point, Direction direct,int count)
        {
            //缓存当前坐标的引用
            ChessPoint tempPoint = Chess.Find(Chess.Find(point).DirectionPoint(direct));

            int index = 0;
            //寻找
            while (index<count)
            {
                if (tempPoint == null)
                    return false;

                else if (tempPoint.Type != ChessType.NULL)
                    return true;

                tempPoint = Chess.Find(tempPoint.DirectionPoint(direct));

                index++;
            }
            return false;
        }

        /// <summary>
        /// 判断某个点是否为活N
        /// </summary>
        /// <param name="point">点坐标</param>
        /// <param name="count">活几</param>
        /// <param name="type">棋子类型</param>
        /// <returns></returns>
        public static Boolean IsActive(Point point,int count, ChessType type)
        {
                if (ChessRule.checkDirectOnChessForActive(point, Direction.Down, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UP, type) + 1 == count)
                    return true;
                if (ChessRule.checkDirectOnChessForActive(point, Direction.DownLeft, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UpRight, type) + 1 == count)
                    return true;
                if (ChessRule.checkDirectOnChessForActive(point, Direction.DownRight, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UpLeft, type) + 1 == count)
                    return true;
                if (ChessRule.checkDirectOnChessForActive(point, Direction.Left, type) + ChessRule.checkDirectOnChessForActive(point, Direction.Right, type) + 1 == count)
                    return true;
            
            return false;
        }

        /// <summary>
        /// 判断某个点是否为单N
        /// </summary>
        /// <param name="point">点坐标</param>
        /// <param name="count">单几</param>
        /// /// <param name="type">棋子类型</param>
        /// <returns></returns>
        public static Boolean IsSingle(Point point, int count, ChessType type)
        {
            if (ChessRule.CheckDirectOnChess(point, Direction.UP, type) + 1 == count)
                return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.UpLeft, type) + 1 == count)
                return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.Left, type) + 1 == count)
                return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.DownLeft, type) + 1 == count)
                return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.Down, type) + 1 == count)
                return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.DownRight, type) + 1 == count)
                return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.Right, type) + 1 == count)
                return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.UpRight, type) + 1 == count)
                return true;


            return false;
        }

        /// <summary>
        /// 判断是否为5连
        /// </summary>
        /// <param name="point"></param>
        /// <returns></returns>
        public static Boolean CheckFiveARow(Point point, ChessType type)
        {
            if (ChessRule.CheckDirectOnChess(point, Direction.Down, type) + ChessRule.CheckDirectOnChess(point, Direction.UP, type) + 1 == 5)
                    return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.DownLeft, type) + ChessRule.CheckDirectOnChess(point, Direction.UpRight, type) + 1 == 5)
                    return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.DownRight, type) + ChessRule.CheckDirectOnChess(point, Direction.UpLeft, type) + 1 == 5)
                    return true;
            if (ChessRule.CheckDirectOnChess(point, Direction.Left, type) + ChessRule.CheckDirectOnChess(point, Direction.Right, type) + 1 == 5)
                    return true;
            
            return false;
        }

        /// <summary>
        /// 计算一个棋子的通用威力
        /// </summary>
        /// <param name="point"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        public static int CommonPower(Point point,ChessType type)
        {
            List<int> powerContainer = new List<int>();

            //计算棋子通用威力
                
            //单二
            if (checkDirectOnChessForSingle(point, Direction.UP, type) + 1 == 2)
                powerContainer.Add(5);
            if (checkDirectOnChessForSingle(point, Direction.UpLeft, type) + 1 == 2)
                powerContainer.Add(5);
            if (checkDirectOnChessForSingle(point, Direction.Left, type) + 1 == 2)
                powerContainer.Add(5);
            if (checkDirectOnChessForSingle(point, Direction.DownLeft, type) + 1 == 2)
                powerContainer.Add(5);
            if (checkDirectOnChessForSingle(point, Direction.Down, type) + 1 == 2)
                powerContainer.Add(5);
            if (checkDirectOnChessForSingle(point, Direction.DownRight, type) + 1 == 2)
                powerContainer.Add(5);
            if (checkDirectOnChessForSingle(point, Direction.Right, type) + 1 == 2)
                powerContainer.Add(5);
            if (checkDirectOnChessForSingle(point, Direction.UpRight, type) + 1 == 2)
                powerContainer.Add(5);

            //单一
            if (checkDirectOnChessForSingle(point, Direction.UP, type) + 1 == 1)
                powerContainer.Add(1);
            if (checkDirectOnChessForSingle(point, Direction.UpLeft, type) + 1 == 1)
                powerContainer.Add(1);
            if (checkDirectOnChessForSingle(point, Direction.Left, type) + 1 == 1)
                powerContainer.Add(1);
            if (checkDirectOnChessForSingle(point, Direction.DownLeft, type) + 1 == 1)
                powerContainer.Add(1);
            if (checkDirectOnChessForSingle(point, Direction.Down, type) + 1 == 1)
                powerContainer.Add(1);
            if (checkDirectOnChessForSingle(point, Direction.DownRight, type) + 1 == 1)
                powerContainer.Add(1);
            if (checkDirectOnChessForSingle(point, Direction.Right, type) + 1 == 1)
                powerContainer.Add(1);
            if (checkDirectOnChessForSingle(point, Direction.UpRight, type) + 1 == 1)
                powerContainer.Add(1);

            //活二
            if (ChessRule.checkDirectOnChessForActive(point, Direction.Down, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UP, type) + 1 == 2)
                powerContainer.Add(7);
            if (ChessRule.checkDirectOnChessForActive(point, Direction.DownLeft, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UpRight, type) + 1 == 2)
                powerContainer.Add(7);
            if (ChessRule.checkDirectOnChessForActive(point, Direction.DownRight, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UpLeft, type) + 1 == 2)
                powerContainer.Add(7);
            if (ChessRule.checkDirectOnChessForActive(point, Direction.Left, type) + ChessRule.checkDirectOnChessForActive(point, Direction.Right, type) + 1 == 2)
                powerContainer.Add(7);

            //活一
            if (ChessRule.checkDirectOnChessForActive(point, Direction.Down, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UP, type) + 1 == 1)
                powerContainer.Add(3);
            if (ChessRule.checkDirectOnChessForActive(point, Direction.DownLeft, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UpRight, type) + 1 == 1)
                powerContainer.Add(3);
            if (ChessRule.checkDirectOnChessForActive(point, Direction.DownRight, type) + ChessRule.checkDirectOnChessForActive(point, Direction.UpLeft, type) + 1 == 1)
                powerContainer.Add(3);
            if (ChessRule.checkDirectOnChessForActive(point, Direction.Left, type) + ChessRule.checkDirectOnChessForActive(point, Direction.Right, type) + 1 == 1)
                powerContainer.Add(3);

            //排序
            powerContainer.Sort();

            //返回最大的两个值和
            return powerContainer[powerContainer.Count - 2] + powerContainer[powerContainer.Count - 1];
        }

        /// <summary>
        /// 判断某一方向是否为N
        /// </summary>
        /// <param name="direct"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        public static int checkDirectOnChessForSingle(Point point, Direction direct, ChessType type)
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
                    return count;
            }
        }
    }
}
