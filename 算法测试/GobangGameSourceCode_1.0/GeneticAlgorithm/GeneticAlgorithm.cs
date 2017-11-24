using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using GoBangProject.structs;

namespace GoBangProject.GeneticAlgorithm
{

    /// <summary>
    /// 创建种群的基因串
    /// </summary>
    public delegate void FitnessFunction(Genome genome);

    public class GeneticAlgorithm
    {

        #region 成员变量
        /// <summary>
        /// 种群变异概率
        /// </summary>
        private double mutationRate;

        /// <summary>
        /// 种群交叉概率
        /// </summary>
        private double crossoverRate;

        /// <summary>
        /// 原始种群数量
        /// </summary>
        private int originalPopulationSize;

        /// <summary>
        /// 最大种群的代数
        /// </summary>
        private int maxGenerationSize;

        /// <summary>
        /// 每个种群的大小
        /// </summary>
        private int genomeSize;

        /// <summary>
        /// 存放种群的容器
        /// </summary>
        private List<Genome> genomeList;

        /// <summary>
        /// 整个种群的适应度和
        /// </summary>
        private double totalFitness;

        /// <summary>
        /// 当前最优的个体
        /// </summary>
        private Genome bestGenome;

        /// <summary>
        /// 下一代种群
        /// </summary>
        public List<Genome> nextGenomeList;

        #endregion

        #region 类变量

        /// <summary>
        /// 计算适应性度量值的函数委托
        /// </summary>
        public static FitnessFunction fitnessCalculator;

        #endregion

        #region 成员变量属性

        public double MutationRate
        {
            get { return mutationRate; }
        }

        public int MaxGenerationSize
        {
            get { return maxGenerationSize; }
        }


        public double CrossoverRate
        {
            get { return crossoverRate; }
        }

        internal List<Genome> GenomeList
        {
            get { return genomeList; }
            set { genomeList = value; }
        }

        public int OriginalPopulationSize
        {
            get { return originalPopulationSize; }
        }

        public int GenomeSize
        {
            get { return genomeSize; }
        }

        public Genome BestGenome
        {
            get
            {
                return bestGenome;
            }
        }

        #endregion


        #region 构造器
        /// <summary>
        /// 默认构造器
        /// 突变概率:5%,交叉概率:80%,原始种群:100,总共代数:2000,基因串长度8
        /// </summary>
        public GeneticAlgorithm()
        {
            this.mutationRate = 0.02;
            this.crossoverRate = 0.8;
            this.originalPopulationSize = 500;
            this.maxGenerationSize = 10000;
            this.genomeSize = 8;
            CreateGenomeList();
        }

        /// <summary>
        /// 参数构造器
        /// </summary>
        /// <param name="crossoverRate">交叉概率</param>
        /// <param name="mutationRate">突变概率</param>
        /// <param name="originalPopulationSize">原始种群数量</param>
        /// <param name="maxGenerationSize">最大种群代数</param>
        /// <param name="genomeSize">每个种群的大小</param>
        public GeneticAlgorithm(double crossoverRate, double mutationRate, int originalPopulationSize, int maxGenerationSize, int genomeSize)
        {
            this.mutationRate = mutationRate;
            this.crossoverRate = crossoverRate;
            this.originalPopulationSize = originalPopulationSize;
            this.maxGenerationSize = maxGenerationSize;
            this.genomeSize = genomeSize;
            CreateGenomeList();
        }

        #endregion

        /// <summary>
        /// 创建原始种群
        /// </summary>
        public void CreateGenomeList()
        {
            genomeList = new List<Genome>();
            for (int i = 0; i < originalPopulationSize; i++)
            {
                genomeList.Add(new Genome(this.genomeSize));
            }
        }
        /// <summary>
        /// 执行遗传算法
        /// </summary>
        public void Start()
        {
             for(int i=0;i<maxGenerationSize;i++)
             {
                 //更新适应度
                 UpdateFitness();
                 //按照适应度排序
                 GenomeList.Sort();

                 ////如果最优个体的适应性度量值=最差个体的适应性度量值
                 if (GenomeList.Count==0 || GenomeList[GenomeList.Count - 1].Fitness == GenomeList[0].Fitness)
                     break;

                 //将第解赋值给最优个体
                 if (Math.Abs(GenomeList[GenomeList.Count - 1].Fitness) < Math.Abs(GenomeList[0].Fitness))
                     this.bestGenome = GenomeList[0];
                 else
                     this.bestGenome = GenomeList[GenomeList.Count - 1];

                 //选择复制
                 Selection();
                 //繁衍子代--交叉
                 Breed();
                 //变异
                 Mutation();
                 //将下一代种群重新赋值给种群容器
                 GenomeList = nextGenomeList;
             }
        }

        /// <summary>
        /// 更新整个种群的适应度和
        /// </summary>
        private void UpdateTotalFitness()
        {
            totalFitness = 0;
            foreach (Genome genome in GenomeList)
                totalFitness += Math.Abs(genome.Fitness);
        }

        /// <summary>
        /// 生成选择遗传的概率,并采用"轮盘赌"模型执行选择操作
        /// 分配更大的"可能被复制"给适应性更高的个体
        /// </summary>
        private void Selection()
        {
            //更新整个种群的适应度和
            UpdateTotalFitness();
            //创建下一代种群容器
            nextGenomeList = new List<Genome>();

            ////最优个体保存原则
            //nextGenomeList.Add(GenomeList[GenomeList.Count - 1]);

            //更新每个个体被选中的概率---排除个体最优
            for (int i = 0; i < GenomeList.Count;i++ )
            {
                //概率执行复制操作
                ProbabilityExcute.Excute((Math.Abs(GenomeList[i].Fitness) / this.totalFitness), delegate()
                {
                    //复制到下一代
                    nextGenomeList.Add(GenomeList[i]);
                });
            }
        }

        /// <summary>
        /// 繁衍子代
        /// </summary>
        private void Breed()
        {
            //概率执行繁衍
            ProbabilityExcute.Excute(crossoverRate, delegate()
            {
                //随机两个父代
                Genome parent1 = GenomeList[ProbabilityExcute.globleRand.Next(0, GenomeList.Count / 2)];

                Genome parent2 = GenomeList[ProbabilityExcute.globleRand.Next(0, GenomeList.Count / 2)];

                //生成子代,并将子代填充到容器中
                Genome[] children = parent1.Crossover(parent2);

                foreach (Genome child in children)
                    nextGenomeList.Add(child);
            });
        }

        /// <summary>
        /// 概率进行个体的变异
        /// </summary>
        private void Mutation()
        {
            //概率执行变异
            ProbabilityExcute.Excute(mutationRate, delegate()
            {
                if (nextGenomeList.Count <= 1)
                    return;
                nextGenomeList[ProbabilityExcute.globleRand.Next(1, nextGenomeList.Count)].Mutate();
            });
        }

        /// <summary>
        /// 更新群体适应度
        /// </summary>
        private void UpdateFitness()
        {
            //计算适应度
            if (fitnessCalculator == null)
                throw new Exception("没有初始化 FitnessFunction 委托");
            foreach (Genome genome in genomeList)
                fitnessCalculator(genome);

            //按照适应度排序
            GenomeList.Sort();
        }

        /// <summary>
        /// 打印当前种群
        /// </summary>
        public void PrintCurrentGenome()
        {
            Console.WriteLine("============================================");
            foreach (Genome genome in genomeList)
            {
                Console.WriteLine("Fitness:" + genome.Fitness);
            }
            Console.WriteLine("============================================");
        }
    }
}
