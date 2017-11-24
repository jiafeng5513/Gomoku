using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace GoBangProject.GeneticAlgorithm
{
    class TestCrossOver
    {
        public static void Test()
        {
            Point[] parent1 = new Point[] { new Point(1, 1), new Point(2, 3), new Point(3, 3), new Point(3, 4), new Point(4, 5) };

            Point[] parent2 = new Point[] { new Point(2, 2), new Point(1, 1), new Point(3, 2), new Point(4, 4), new Point(5, 5) };

            Genome parent1Genome = new Genome(parent1);
            Genome parent2Genome = new Genome(parent2);

            DateTime start = DateTime.Now;
            CrossOver cross = new CrossOver(parent1, parent2,2);
            Genome[] result = cross.getChildren();
            DateTime end = DateTime.Now;

            foreach (Genome genome in result)
            {
                Console.WriteLine("***********************************************");
                foreach (Point point in genome.Genes)
                {
                    Console.WriteLine(point.ToString());
                }
                Console.WriteLine("***********************************************");
            }

            TimeSpan ts = end.Subtract(start);//时间差        
            Console.WriteLine("Time:{0}", ts.TotalMilliseconds);

            List<Point> list = new List<Point>();
            Point a = new Point(1, 1);
            list.Add(a);

            Point b = new Point(1, 1);
            Console.WriteLine(list.Contains(b));
        }
    }
}
