using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.IO;

namespace FiveStone
{
    public partial class Form1 : Form
    {
        //public bool first=true;
        public Form1()
        {
            InitializeComponent();
        }

        public Boards bd;

        private void Form1_Load(object sender, EventArgs e)
        {
            bd = new Boards(this.CreateGraphics());
            bd.ClearBoards(); 
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            bd.DrawBoard();
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.X < 680 && e.Y < 680)
            {
                int m = (int)(e.X / 42);
                int n = (int)(e.Y / 42);
                if (m < 0)
                { m = 0; }
                if (n < 0)
                { n = 0; }
                if (m > 14)
                { m = 14; }
                if (n > 14)
                { n = 14; }
                label1.Text = "X：" + m.ToString() + "  Y：" + n.ToString();
                toolStripStatusLabel1.Text = "落子点：X " + m.ToString() + " Y " + n.ToString() + "";
            }
        }

        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            bd.PersonPut(e.X, e.Y);

            if (historylist.Items.Count == 0)
            {
                historylist.Items.Add(bd.st.GetPerson());
                historylist.Items.Add(bd.st.GetPC());
            }
            else
            {
                if (historylist.Items[historylist.Items.Count-1].ToString() != bd.st.GetPC() && historylist.Items[historylist.Items.Count - 2].ToString() != bd.st.GetPerson())
                {
                    historylist.Items.Add(bd.st.GetPerson());
                    historylist.Items.Add(bd.st.GetPC());
                }
            }
        }

        private void 新游戏ToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            if (label2.Text == "0")
            {
                bd.Start(false);
            }
            else
            {
                bd.Start(true);
            }
            historylist.Items.Clear();
        }

        private void 玩家先ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!玩家先ToolStripMenuItem.Checked)
            {
                bd.Start(true);
                historylist.Items.Clear();
                玩家先ToolStripMenuItem.Checked = true;
                电脑先ToolStripMenuItem.Checked = false;
                black.Text = "玩家";
                white.Text = "计算机";
                label2.Text = "1";
            }
        }

        private void 电脑先ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!电脑先ToolStripMenuItem.Checked)
            {
                bd.Start(false);
                historylist.Items.Clear();
                电脑先ToolStripMenuItem.Checked = true;
                玩家先ToolStripMenuItem.Checked = false;
                black.Text = "计算机";
                white.Text = "玩家";
                label2.Text = "0";
            }
        }

        private void 退出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
