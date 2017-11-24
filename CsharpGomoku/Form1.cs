using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using GoBangProject.structs;
using GoBangProject.GeneticAlgorithm;
//Download by http://www.codesc.net
namespace GoBangProject
{
    public partial class Form1 : Form
    {
        public static Form1 formInstance;
        public Form1()
        {
            InitializeComponent();
            formInstance = this;
        }

        private void startGameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.tableLayoutPanel7.Visible = false;
            initChess();
            this.tableLayoutPanel7.Visible = true;
        }

        /// <summary>
        /// 对比赛进行初始化
        /// </summary>
        private void initChess()
        {
            this.tableLayoutPanel7.Controls.Clear();
            //初始化棋盘
            for (int rowIndex = 0; rowIndex < Chess.GlobleChess.GetLength(0); rowIndex++)
            {
                for (int columnIndex = 0; columnIndex < Chess.GlobleChess.GetLength(1); columnIndex++)
                {
                    Chess.GlobleChess[rowIndex,columnIndex] = ChessPoint.CreateChessPoint(new Point(columnIndex,rowIndex), ChessType.NULL);
                    this.tableLayoutPanel7.Controls.Add(Chess.GlobleChess[rowIndex, columnIndex], columnIndex + 1, rowIndex + 1);
                }
            }

            //初始化历史记录
            Conf.globleRoute = new List<HistoryRecord>();

            //初始化历史记录列表
            this.routeListBox.Items.Clear();

            //初始化先行方
            Conf.CurrentTurn = ChessType.BLACK;

            //初始化缓存棋步
            Conf.LastPoint = null;

            //打开组件
            this.AbleAllCompents(true);
        }

        /// <summary>
        /// 添加记录
        /// </summary>
        /// <param name="text"></param>
        public void AddRecordToList(String text)
        {
            this.routeListBox.Items.Add(text);
            this.routeListBox.SelectedIndex = routeListBox.Items.Count - 1;

            //如果棋盘下满,则平局
            if (this.routeListBox.Items.Count >= 125)
                ChessRule.WinPeace();
        }

        /// <summary>
        /// 认输按钮的处理事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            if (Conf.CurrentTurn == ChessType.BLACK)
                MessageBox.Show(ChessType.WHITE + "赢得比赛","认输不是好习惯咯~~");
            else
                MessageBox.Show(ChessType.BLACK + "赢得比赛", "认输不是好习惯咯~~");
            AbleAllCompents(false);
        }

        /// <summary>
        /// 启用或禁用所有组件
        /// </summary>
        public void AbleAllCompents(Boolean enable)
        {
            this.toolStripStatusLabel1.Text = "请稍候.....";

            this.tableLayoutPanel7.Enabled = enable;

            this.routeListBox.Enabled = enable;

            this.button1.Enabled = enable;

            this.button2.Enabled = enable;

            if (enable)
                this.toolStripStatusLabel1.Text = "开始游戏,"+Conf.CurrentTurn+" 先行";
            else
                this.toolStripStatusLabel1.Text = "等待开始..";
        }

        /// <summary>
        /// 设置状态栏的文本值
        /// </summary>
        public String StateText
        {
            get
            {
                return this.toolStripStatusLabel1.Text;
            }
            set
            {
                this.toolStripStatusLabel1.Text = value;
            }
        }
        /// <summary>
        /// 记录选择的事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void routeListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.routeListBox.SelectedIndex < 0 || this.routeListBox.SelectedIndex >= Conf.globleRoute.Count)
            {
                this.routeListBox.ClearSelected();
                return;
            }

            //清空黑方记录的选择
            for (int i = 0; i < this.routeListBox.Items.Count; i++)
                Conf.globleRoute[i].Point.HistorySelect = false;

            Conf.globleRoute[this.routeListBox.SelectedIndex].Point.HistorySelect = true;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        /// <summary>
        /// 悔棋按钮的处理事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            if (this.routeListBox.SelectedIndex < 0)
            {
                MessageBox.Show("未选择悔棋的棋步","出错咯~");
                return;
            }

            if ((Conf.CurrentTurn == ChessType.BLACK && this.routeListBox.SelectedItem.ToString().Contains(ChessType.WHITE.ToString())) || (Conf.CurrentTurn == ChessType.WHITE && this.routeListBox.SelectedItem.ToString().Contains(ChessType.BLACK.ToString())))
            {
                MessageBox.Show("只可以选择自己的棋子进行悔棋","出错咯~");
                return;
            }

            //删除历史记录
            ChessRule.RegretPoint(this.routeListBox.SelectedIndex);

            //删除显示的列表的值
            int index = this.routeListBox.SelectedIndex;

            for (int i = this.routeListBox.Items.Count - 1; i >= index; i--)
                this.routeListBox.Items.RemoveAt(i);

            //重新选中最后一步
            this.routeListBox.SelectedIndex = routeListBox.Items.Count - 1;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (MessageBox.Show("确认要退出?", "退出", MessageBoxButtons.YesNo) != DialogResult.Yes)
                e.Cancel = true;
            return;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("谁让你乱点的....后果自负袄~","好奇害死猫~");
            this.Visible = false;
            Funny.StartFunny();
        }

        private void 开始游戏人机对战ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AIOperator.AIType = ChessType.WHITE;
            this.tableLayoutPanel7.Visible = false;
            initChess();
            this.tableLayoutPanel7.Visible = true;
        }
    }
}
