using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GoBangProject.GeneticAlgorithm
{
    /// <summary>
    /// 进行交叉操作的算法类
    /// </summary>
    public class CrossOver
    {
        /// <summary>
        /// 父1代
        /// </summary>
        private Point[] parent1;

        /// <summary>
        /// 父2代
        /// </summary>
        private Point[] parent2;

        /// <summary>
        /// 子1代
        /// </summary>
        private Point[] child1;

        /// <summary>
        /// 子2代
        /// </summary>
        private Point[] child2;

        /// <summary>
        /// 随机的固定不变基因
        /// </summary>
        private Point[] staticGens;

        /// <summary>
        /// 随机的固定基因数量
        /// </summary>
        private int randGensCount;


        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="parent1"></param>
        /// <param name="parent2"></param>
        /// <param name="count"></param>
        public CrossOver(Point[] parent1, Point[] parent2,int count)
        {
            this.parent1 = parent1;
            this.parent2 = parent2;

            //foreach (Point point in parent2)
            //{
            //    if (point == Conf.NullPoint)
            //    {
            //        Console.WriteLine("child error");
            //        throw new Exception("父代错误");
            //    }
            //}

            //foreach (Point point in parent1)
            //{
            //    if (point == Conf.NullPoint)
            //    {
            //        Console.WriteLine("child error");
            //        throw new Exception("父代错误");
            //    }
            //}

            child1 = new Point[parent1.Length];
            child2 = new Point[parent2.Length];

            staticGens = new Point[count];

            randGensCount = count;

            //产生随机的固定不变基因
            generateRandGenes();
        }

        /// <summary>
        /// 获得子代
        /// </summary>
        /// <returns></returns>
        public Genome[] getChildren()
        {
            Point[] filterParent1Container = filterParent1();
            Point[] filterParent2Container = filterParent2();

            //填充子一代
            int child1Index = 0;
            for (int j = 0; child1Index < child1.Length; child1Index++)
            {
                if (child1[child1Index] != Conf.NullPoint)
                    continue;
                else
                {
                    if (j >= filterParent2Container.Length)
                        child1[child1Index] = parent1[child1Index];
                    else
                    {
                        child1[child1Index] = filterParent2Container[j];
                        j++;
                    }
                }
            }
            //填充子二代
            int child2Index = 0;
            for (int j = 0; child2Index < child2.Length; child2Index++)
            {
                if (child2[child2Index] != Conf.NullPoint)
                    continue;
                else
                {
                    if (j >= filterParent1Container.Length)
                        child2[child2Index] = parent2[child2Index];
                    else
                    {
                        child2[child2Index] = filterParent1Container[j];
                        j++;
                    }
                }
            }

            return new Genome[] { new Genome(child1), new Genome(child2) };
        }

        /// <summary>
        /// 过滤父一代
        /// </summary>
        private Point[] filterParent1()
        {
            //将父代中的固定基因排除
            List<Point> filterContainer = new List<Point>();

            for (int i = 0; i < parent1.Length; i++)
            {
                //向父一代过滤容器中添加该基因
                filterContainer.Add(parent1[i]);
                //子一代基因置空
                child1[i] = Conf.NullPoint;
                for (int randGenIndex = 0; randGenIndex < randGensCount; randGenIndex++)
                {
                    //如果父一代该基因为被固定的基因
                    if (staticGens[randGenIndex] == parent1[i])
                    {
                        //填入子一代
                        child1[i] = staticGens[randGenIndex];

                        //从父一代过滤容器中删除该基因
                        filterContainer.Remove(parent1[i]);

                        break;
                    }
                }
            }
            return filterContainer.ToArray();
        }

        /// <summary>
        /// 过滤父二代
        /// </summary>
        private Point[] filterParent2()
        {
            //将父代中的固定基因排除
            List<Point> filterContainer = new List<Point>();

            for (int i = 0; i < parent1.Length; i++)
            {
                //向父一代过滤容器中添加该基因
                filterContainer.Add(parent2[i]);
                //子一代基因置空
                child2[i] = Conf.NullPoint;
                for (int randGenIndex = 0; randGenIndex < randGensCount; randGenIndex++)
                {
                    //如果父一代该基因为被固定的基因
                    if (staticGens[randGenIndex] == parent2[i])
                    {
                        //填入子二代
                        child2[i] = staticGens[randGenIndex];

                        //从父二代过滤容器中删除该基因
                        filterContainer.Remove(parent2[i]);

                        break;
                    }
                }
            }

            return filterContainer.ToArray();
        }

        /// <summary>
        /// 随机产生固定基因
        /// </summary>
        private void generateRandGenes()
        {
            Point[] tempParent = null;
            for(int i=0;i<randGensCount;i++)
            {
                if (i % 2 == 0)
                    tempParent = this.parent1;
                else
                    tempParent = this.parent2;

                staticGens[i] = tempParent[ProbabilityExcute.globleRand.Next(0, tempParent.Length)];
            }
        }
    }
}
