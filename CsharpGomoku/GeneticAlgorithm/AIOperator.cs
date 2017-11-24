using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using GoBangProject.structs;

namespace GoBangProject.GeneticAlgorithm
{
    /// <summary>
    /// 人工智能操作者
    /// </summary>
    public class AIOperator
    {
        /// <summary>
        /// AI玩家
        /// </summary>
        public static AIOperator AIPlayer = new AIOperator();

        /// <summary>
        /// 人工智能的棋子类型
        /// </summary>
        public static ChessType AIType = ChessType.NULL;

        /// <summary>
        /// 执行下棋操作
        /// </summary>
        public void Excute()
        {
            //初始化各种函数委托
            InitHandle();

            //实例化遗传算法
            GeneticAlgorithm ga = new GeneticAlgorithm();
            //运算
            ga.Start();
            //下棋
            if (ga.BestGenome.Fitness > 0)
                Chess.Find(ga.BestGenome.Genes[0]).ChessPoint_Click(null, null);
            else
                Chess.Find(ga.BestGenome.Genes[ga.BestGenome.BestGenIndex]).ChessPoint_Click(null, null);
        }

        /// <summary>
        /// 初始化所有委托
        /// </summary>
        public void InitHandle()
        {
            //如果两项委托为空
            if (GeneticAlgorithm.fitnessCalculator == null || Genome.genomeCreater == null)
            {

                //适应度度量函数
                GeneticAlgorithm.fitnessCalculator = delegate(Genome gene)
                                {
                                    //打开人工智能探路标记
                                    Conf.AIRouterState = true;

                                    //当前串中最大的威力
                                    double maxPower = double.MinValue;

                                    //当前串中最小的威力
                                    double minPower = double.MaxValue;

                                    //当前串中最大的威力棋子的位置
                                    int maxPowerIndex = 0;

                                    //当前串中最小的威力棋子的位置
                                    int minPowerIndex = 0;

                                    //设定当前添加的棋子类型
                                    ChessType currentType = AIType;

                                    for (int i = 0; i < gene.Genes.Length; i++)
                                    {
                                        //模拟更新棋盘中的棋子类型
                                        Chess.UpdateChessTypeForAI(gene.Genes[i], currentType);

                                        //计算单子威力*位权
                                        double tempPower = SinglePower(gene.Genes[i], currentType) * 1 * Math.Pow(0.96, i);

                                        if (tempPower > maxPower)
                                        {
                                            maxPowerIndex = i;
                                            maxPower = tempPower;
                                        }

                                        if (tempPower < minPower)
                                        {
                                            minPowerIndex = i;
                                            minPower = tempPower;
                                        }

                                        //当前的最大威力值>权值*100  退出循环
                                        if (maxPower > (100 * Math.Pow(0.96, i)))
                                            break;

                                        //转换下棋方,currentTpye变换
                                        if (currentType == ChessType.BLACK)
                                            currentType = ChessType.WHITE;
                                        else
                                            currentType = ChessType.BLACK;
                                    }

                                    //赋值适应度
                                    if (Math.Abs(minPower) > Math.Abs(maxPower))
                                    {
                                        gene.BestGenIndex = minPowerIndex;
                                        gene.Fitness = minPower;
                                    }
                                    else
                                    {
                                        gene.Fitness = maxPower;
                                        gene.BestGenIndex = maxPowerIndex;
                                    }

                                    //恢复对棋盘的改变
                                    for (int i = 0; i < gene.Genes.Length; i++)
                                    {
                                        Chess.UpdateChessTypeForAI(gene.Genes[i], ChessType.NULL);
                                    }

                                    //关闭人工智能探路标记
                                    Conf.AIRouterState = false;
                                };

                //创建随机基因组的函数
                Genome.genomeCreater = createGensHandle;
            }
        }

        /// <summary>
        /// 计算单子威力
        /// </summary>
        /// <param name="point"></param>
        /// <param name="type"></param>
        /// <returns></returns>
        private int SinglePower(Point point, ChessType type)
        {

            //如果己方为黑子
            if (AIType == ChessType.BLACK && AIType == type)
            {
                //判断禁手
                if (ChessRule.CheckAbort3_3())
                    return int.MinValue;

                if (ChessRule.CheckAbort4_4())
                    return int.MinValue;

                if (ChessRule.CheckAbortLong())
                    return int.MinValue;

            }
            //判断是否为5连
            if (AIRule.CheckFiveARow(point, type))
                return (type == AIType) ? 100 : -100;

            //判断是否为活4
            if (AIRule.IsActive(point, 4, type))
                return (type == AIType) ? 100 : -100;

            //判断是否为单4
            if (AIRule.IsSingle(point, 4, type))
                return (type == AIType) ? 90 : -90;

            //判断是否为单3
            if (AIRule.IsSingle(point, 3, type))
                return (type == AIType) ? 9 : -9;

            //判断是否为活3
            if (AIRule.IsActive(point, 3, type))
                return (type == AIType) ? 90 : -90;
            int commonPower = AIRule.CommonPower(point, type);

            return (type == AIType) ? commonPower : (commonPower * -1);
        }

        /// <summary>
        /// 检查四个方向上,是否有子
        /// </summary>
        /// <param name="point"></param>
        /// <returns></returns>
        public static Boolean checkPoint(Point point, int count)
        {
            return AIRule.CheckDirectOnChess(point, Direction.Down, count) ||
            AIRule.CheckDirectOnChess(point, Direction.DownLeft, count) ||
            AIRule.CheckDirectOnChess(point, Direction.DownRight, count) ||
            AIRule.CheckDirectOnChess(point, Direction.Left, count) ||
            AIRule.CheckDirectOnChess(point, Direction.Right, count) ||
            AIRule.CheckDirectOnChess(point, Direction.UP, count) ||
            AIRule.CheckDirectOnChess(point, Direction.UpLeft, count) ||
            AIRule.CheckDirectOnChess(point, Direction.UpRight, count);
        }

        /// <summary>
        /// 创建基因用的函数
        /// </summary>
        /// <param name="count"></param>
        /// <returns></returns>
        private Point[] createGensHandle(int count)
        {
            List<Point> returnList = new List<Point>();
            Point point = Conf.NullPoint;
            for (int i = 0; i < count;i++ )
            {
                while (true)
                {
                    point = ProbabilityExcute.CreateRandGene();

                    //如果随机的棋子为空,没有重复生成点坐标,并周围1个单元格内有其他棋子.则停止循环
                    if (!returnList.Contains(point) && Chess.Find(point).Type == ChessType.NULL && checkPoint(point, 2))
                    {
                        returnList.Add(point);
                        break;
                    }
                }
            }
            return returnList.ToArray();
        }
    }
}
