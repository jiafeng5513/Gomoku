using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using GoBangProject.structs;
using GoBangProject.GeneticAlgorithm;

namespace GoBangProject
{
    public class Conf
    {
        /// <summary>
        /// 棋盘宽度
        /// </summary>
        public static readonly  int ChessWidth = 15;

        /// <summary>
        /// 棋盘高度
        /// </summary>
        public static readonly int ChessHeight = 15;

        /// <summary>
        /// 作为全局空标记
        /// </summary>
        public static readonly Point NullPoint = new Point(-1, -1);

        /// <summary>
        /// 记录当前下棋方
        /// </summary>
        public static ChessType CurrentTurn = ChessType.BLACK;

        /// <summary>
        /// 记录上一步下的位置
        /// </summary>
        public static ChessPoint LastPoint;

        /// <summary>
        /// 下棋的历史记录
        /// </summary>
        public static List<HistoryRecord> globleRoute;

        /// <summary>
        /// AI智能探测棋路的状态
        /// </summary>
        public static Boolean AIRouterState = false;
    }
}
