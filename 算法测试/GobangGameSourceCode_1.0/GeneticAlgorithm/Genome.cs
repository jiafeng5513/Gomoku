using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GoBangProject.GeneticAlgorithm
{
    /// <summary>
    /// 创建个体的基因串
    /// </summary>
    public delegate Point[] CreateGenome(int count);

    /// <summary>
    /// 个体的描述
    /// </summary>
    public class Genome :IComparable<Genome>
    {
        /// <summary>
        /// 自己设定基因串
        /// </summary>
        /// <param name="genes">基因串</param>
        public Genome(Point[] genes)
        {
            this.genes = genes;
        }

        /// <summary>
        /// 自动生成基因串
        /// </summary>
        public Genome(int count)
        {
            if (genomeCreater == null)
                throw new Exception("没有初始化 CreateGenome 委托");

            this.genes = genomeCreater(count);
        }

        #region 成员变量

        //该个体的适应度
        private double fitness;

        //基因组
        private Point[] genes;

        //最优基因
        private Point bestGene = Conf.NullPoint;

        //最优基因所在位置
        private int bestGenIndex = 0;

        #endregion

        #region 类变量

        //创建基因组的委托
        public static CreateGenome genomeCreater;

        #endregion

        #region 成员属性

        public double Fitness
        {
            get { return fitness; }
            set { fitness = value; }
        }

        public int BestGenIndex
        {
            get { return bestGenIndex; }
            set { bestGenIndex = value; }
        }

        public Point[] Genes
        {
            get { return genes; }
        }

        public Point BestGene
        {
            get { return bestGene; }
            set { bestGene = value; }
        }

        #endregion

        #region 基因的相关操作

        /// <summary>
        /// 基因交叉
        /// </summary>
        /// <param name="parent2">第二个父代</param>
        /// <returns>两个子代</returns>
        public Genome[] Crossover(Genome parent2)
        {
            CrossOver cross = new CrossOver(this.genes, parent2.Genes,3);
            return cross.getChildren();

        }

        /// <summary>
        /// 判断基因在基因串中位置
        /// </summary>
        /// <param name="point">基因</param>
        /// <param name="genome">基因串</param>
        /// <returns>位置</returns>
        public static  int FindGens(Point point,Genome genome)
        {
            for(int i=0;i<genome.Genes.Length;i++)
            {
                if (genome.Genes[i] == point)
                    return i;
            }
            return -1;
        }

        /// <summary>
        /// 基因变异
        /// </summary>
        public void Mutate()
        {
            //随机变异次数
            int count = ProbabilityExcute.globleRand.Next(1, 4);

            for(int i=0;i<count;i++)
            {
                //随机两个位置
                int a = ProbabilityExcute.globleRand.Next(0, genes.Length);
                int b = ProbabilityExcute.globleRand.Next(0, genes.Length);

                //两个基因交换位置
                Point tempGene = genes[a];
                genes[a] = genes[b];
                genes[b] = tempGene;
            }
        }
        #endregion

        #region 比较接口

        int IComparable<Genome>.CompareTo(Genome other)
        {
            if (this.Fitness > other.Fitness)
                return 1;
            else if (this.Fitness == other.Fitness)
                return 0;
            else
                return -1;
        }

        #endregion

        #region 重载ToString

        public override string ToString()
        {
            StringBuilder buff = new StringBuilder();

            buff.Append("\n*****************Fitness:" + this.fitness + "*******************");
            foreach (Point gene in genes)
            {
                buff.Append("\nPoint:" + gene.ToString() + "\n");
            }
            buff.Append("*****************Fitness:" + this.fitness + "*******************\n");
            return buff.ToString();
        }
        #endregion
    }
}
