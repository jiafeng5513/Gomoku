using System;
using System.Collections.Generic;
using System.Windows.Forms;
using GoBangProject.GeneticAlgorithm;
//Download by http://www.codesc.net
namespace GoBangProject
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());

            //TestCrossOver.Test();
        }
    }
}
