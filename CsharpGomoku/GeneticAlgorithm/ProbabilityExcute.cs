using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GoBangProject.GeneticAlgorithm
{

    /// <summary>
    /// 要概率执行的函数委托
    /// </summary>
    public delegate void ExcuteAsProbability();

    /// <summary>
    /// 按照固定概率执行程序
    /// </summary>
    public class ProbabilityExcute
    {
        public static Random globleRand = new Random();

        public static void Excute(double rate,ExcuteAsProbability handle)
        {
            //转换概率值
            double tempRate = rate;
            int randMAX = 1;
            while (tempRate < 10 && tempRate!=0)
            {
                tempRate = tempRate * 10;
                randMAX = randMAX * 10;
            }

            //如果发生概率事件,执行委托函数
            if (globleRand.Next(1, randMAX + 1) <= Convert.ToInt32(tempRate))
                handle();
        }

        /// <summary>
        /// 创建随机的基因
        /// </summary>
        /// <returns></returns>
        public static Point CreateRandGene()
        {
            return new Point(globleRand.Next(0, Conf.ChessWidth), globleRand.Next(0,Conf.ChessHeight));
        }
    }
}
