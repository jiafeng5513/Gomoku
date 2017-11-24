using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GoBangProject.structs
{
    public class Chess
    {
        public static ChessPoint[,] GlobleChess = new ChessPoint[15, 15];

        /// <summary>
        /// 在当前棋盘寻找一个点
        /// </summary>
        /// <param name="point">点坐标</param>
        /// <returns></returns>
        public static ChessPoint Find(Point point)
        {
            if (point == Conf.NullPoint)
                return null;
            return GlobleChess[point.Y,point.X];
        }

        /// <summary>
        /// 为人工智能开放更新棋子类型的方法------UI界面不改变
        /// </summary>
        /// <param name="point">坐标</param>
        /// <param name="type">棋子类型</param>
        public static void UpdateChessTypeForAI(Point point,ChessType type)
        {
            if (point == Conf.NullPoint)
                throw new Exception("坐标为空");

            if (!Conf.AIRouterState)
                throw new Exception("AI探路标记未打开");

            GlobleChess[point.Y, point.X].Type = type;
        }
    }
}
