using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using GoBangProject.GeneticAlgorithm;

namespace GoBangProject.structs
{
    /// <summary>
    /// 棋子类型
    /// </summary>
    public enum ChessType
    {
        BLACK,
        WHITE,
        NULL
    }
    public class ChessPoint : Panel
    {

        public static ChessPoint CreateChessPoint(Point point,ChessType type)
        {
            ChessPoint pointInstance = new ChessPoint();

            pointInstance.Point = point;
            pointInstance.Type = type;

            return pointInstance;
        }

        #region 成员变量

        private Point point;

        private ChessType type;

        private Boolean isSelect;

        #endregion

        /// <summary>
        /// 私有构造函数
        /// </summary>
        private ChessPoint()
        {
            //设置棋子的大小
            this.Size = new System.Drawing.Size(35, 35);

            //设置棋子的背景
            this.BackColor = System.Drawing.Color.Transparent;

            //设置鼠标单击事件
            this.Click += new EventHandler(ChessPoint_Click);

            //设置该位置是否被选中
            isSelect = false;

            //设置双缓存
            this.DoubleBuffered = true;
        }


       


        #region 属性
        public ChessType Type
        {
            get { return type; }
            set
            {
                type = value;
                //如果为AI探测棋路,不进行UI界面的修改
                if (Conf.AIRouterState)
                    return;
                checkType();
            }
        }

        public Point Point
        {
            get { return point; }
            set
            {
                point = value;
            }
        }
        #endregion


        /// <summary>
        /// 通过检测type属性,设置背景图片
        /// </summary>
        private void checkType()
        {
            if (this.type == ChessType.BLACK)
                this.BackgroundImage = global::GoBangProject.Properties.Resources.black;
            else if (this.type == ChessType.WHITE)
                this.BackgroundImage = global::GoBangProject.Properties.Resources.white;
            else
                this.BackgroundImage = null;
        }

        /// <summary>
        /// 通过检测point属性,探测该点周围的点
        /// </summary>
        public  Point DirectionPoint(Direction direct)
        {

            //探测上坐标
            if(direct == Direction.UP)
            {
                if (this.point.Y <= 0)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X, this.point.Y - 1);
            }

            //探测右上坐标
            if (direct == Direction.UpRight)
            {
                if (this.point.X >= Conf.ChessWidth - 1 || this.point.Y <= 0)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X + 1, this.point.Y - 1);
            }

            //探测右坐标
            if (direct == Direction.Right)
            {
                if (this.point.X >= Conf.ChessWidth - 1)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X + 1, this.point.Y);
            }

            //探测右下坐标
            if (direct == Direction.DownRight)
            {
                if (this.point.X >= Conf.ChessWidth - 1 || this.point.Y >= Conf.ChessHeight - 1)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X + 1, this.point.Y + 1);
            }

            //探测下坐标
            if (direct == Direction.Down)
            {
                if (this.point.Y >= Conf.ChessHeight - 1)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X, this.point.Y + 1);
            }

            //探测左下坐标
            if (direct == Direction.DownLeft)
            {
                if (this.point.X <= 0 || this.point.Y >= Conf.ChessHeight - 1)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X - 1, this.point.Y + 1);
            }

            //探测左坐标
            if (direct == Direction.Left)
            {
                if (this.point.X <= 0)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X - 1, this.point.Y);
            }

            //探测左上坐标
            if (direct == Direction.UpLeft)
            {
                if (this.point.X <= 0 || this.point.Y <= 0)
                    return Conf.NullPoint;
                else
                    return new Point(this.point.X - 1, this.point.Y - 1);
            }
            return Conf.NullPoint;
        }

        /// <summary>
        /// 鼠标单击的处理事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void ChessPoint_Click(object sender, EventArgs e)
        {
            if (this.Type != ChessType.NULL)
                return;

            //设置该棋子颜色为当前下棋方的棋子颜色
            this.Type = Conf.CurrentTurn;

            //缓存该步棋子的点
            Conf.LastPoint = this;

            //判断输赢
            if (ChessRule.CheckWin())
            {
                ChessRule.WinShow();
                AIOperator.AIType = ChessType.NULL;
                return;
            }

            //交换下棋方
            ChessRule.ChangeType();

            //设置状态栏
            Form1.formInstance.StateText = Conf.CurrentTurn + " 下棋";

            //人工智能
            if (Conf.CurrentTurn == AIOperator.AIType)
                AIOperator.AIPlayer.Excute();
        }

        /// <summary>
        /// 设置该棋子是否被选中
        /// </summary>
        public Boolean HistorySelect
        {
            get
            {
                return this.isSelect;
            }
            set
            {
                if(value)
                    //设置棋子的背景
                    this.BackColor = System.Drawing.Color.Red;
                else
                    //设置棋子的背景
                    this.BackColor = System.Drawing.Color.Transparent;
                this.Refresh();
                this.isSelect=value;
            }
        }

    }
}
