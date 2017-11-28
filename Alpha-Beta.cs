/*
这里用来保存转换出来的C#代码
*/

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;

namespace FiveStone
{
    /*
     * 五子棋AI
     */

    using System;
    using System.Collections.Generic;
    //哈希表中元素的结构定义
    enum ENTRY_TYPE { exact, lower_bound, upper_bound };

    struct HashItem
    {
        public Int64 checksum;           //64位校验码
        public ENTRY_TYPE entry_type;    //数据类型
        public short depth;              //取得此值时的层次
        public short eval;                //节点的值
    }

    struct POINT
    {
        public int x;
        public int y;
    }
    //用以表示棋子位置的结构
    struct STONEPOS
    {
        public int x;
        public int y;
    }
    //走法
    struct MOVESTONE
    {
        public int nRenjuID;
        public POINT ptMovePoint;
    }

    struct STONEMOVE
    {
        public STONEPOS StonePos;//棋子位置
        public int Score;              //走法的分数 
    }



    class AI
    {
        private static int GRID_NUM = 15; //棋盘15条线
        private static int GRID_COUNT = 225; //棋盘上交点总数
        int[,] m_RenjuBoard = new int[GRID_NUM, GRID_NUM]; //棋盘数组，用于显示棋盘
        int m_nMaxDepth; //当前搜索的最大搜索深度
        uint m_HashKey32; //32位哈希值
        Int64 m_HashKey64; //64 位哈希值
        int count = 0; //全局变量,用以统计估值函数的执行遍数
        int m_nMoveCount; //此变量用以记录走法的总数
        STONEMOVE[] m_TargetBuff = new STONEMOVE[225]; //排序用的缓冲队列

        List<HashItem>[] m_pTT = new List<HashItem>[10]; //置换表

        //存放全部分析结果的数组,有三个维度,用于存放水平、垂直、左斜、右斜 4 个方向上所有棋型分析结果
        private int[,,] TypeRecord = new int[GRID_NUM, GRID_NUM, 4];

        int[,] TypeCount = new int[2, 20]; //存放统记过的分析结果的数组,[棋子颜色,20种棋形之一]
        int[] m_LineRecord = new int[30]; //存放AnalysisLine分析结果的数组
        int[,] CurPosition = new int[GRID_NUM, GRID_NUM]; //搜索时用于当前节点棋盘状态的数组
        STONEMOVE[,] m_MoveList = new STONEMOVE[10, 225]; //用以记录走法的数组
        int[,] m_HistoryTable = new int[GRID_NUM, GRID_NUM]; //历史得分表
        int[,,] m_nHashKey32 = new int[15, 225, 225]; //32位随机树组，用以生成32位哈希值
        Int64[,,] m_ulHashKey64 = new Int64[15, 225, 225]; //64位随机树组，用以生成64位哈希值
        STONEMOVE m_cmBestMove; //记录最佳走法的变量
        const int MaxFiveChainCount = 572; // 15*15共有572种五子连珠的可能性

        int _cgrade, _pgrade;
        int _icount, _m, _n;

        int _mat, _nat, _mde, _nde;

        //位置分
        int[,] PosValue =
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
            {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
            {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
            {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
            {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
            {0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0},
            {0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
            {0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
            {0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
            {0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
            {0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        //棋形
        internal enum Pattern
        {
            TOBEANALSIS = 0, //尚未分析
            ANALSISED = 1, //已分析过的
            NOTYPE = 2, //未定义棋形
            Long = 3, /*100000 */
            Five = 4, /* 100000*/
            Alive4 = 5, /* 10000*/
            DoubleRush4 = 6, /* 10000*/
            Rush4Alive3 = 7, /* 10000*/
            DoubleAlive3 = 8, /*5000 */
            Alive3Sleep3 = 9, /* 1000*/
            Sleep4 = 10, /* 500*/
            Alive3 = 11, /* 200*/
            DoubleAlive2 = 12, /* 100*/
            Sleep3 = 13, /* 50*/
            Alive2Sleep2 = 14, /* 10*/
            Alive2 = 15, /* 5*/
            Sleep2 = 16, /* 3*/
            Dead4 = 17, /* -5*/
            Dead3 = 18, /* -5*/
            Dead2 = 19 /* -5*/
        };

        //棋子
        internal enum Piece
        {
            Black = 0, //黑棋
            White = 1, //白棋
            NULL = 2 //没有棋子
        }

        int colour; //AI棋子颜色
        int m_nUserStoneColor; //用户棋子的颜色
        int m_nSearchDepth = 4; //AI难度等级设置 
        private bool SL;
        Random m_randomGenerator = new Random(); //随机数发射器

        int X, Y; //AI输出落子位置 
        //最大搜索深度     

        /// <summary>
        /// 查询哈希表
        /// </summary>
        /// <param name="alpha"></param>
        /// <param name="beta"></param>
        /// <param name="depth"></param>
        /// <param name="TableNo">key</param>
        /// <returns></returns>
        int LookUpHashTable(int alpha, int beta, int depth, int TableNo)
        {
            int x;
            HashItem pht;

            //计算二十位哈希地址，如果读者设定的哈希表大小不是 1M*2 的,
            //而是 TableSize*2，TableSize为读者设定的大小
            //则需要修改这一句为m_HashKey32% TableSize
            //下一个函数中这一句也一样
            x = (int) m_HashKey32 & 0xFFFFF;
            pht = m_pTT[TableNo][x]; //取到具体的表项指针

            if (pht.depth >= depth && pht.checksum == m_HashKey64)
            {
                switch (pht.entry_type) //判断数据类型
                {
                    case ENTRY_TYPE.exact: //确切值
                        return pht.eval;

                    case ENTRY_TYPE.lower_bound: //下边界
                        if (pht.eval >= beta)
                            return pht.eval;
                        else
                            break;

                    case ENTRY_TYPE.upper_bound: //上边界
                        if (pht.eval <= alpha)
                            return pht.eval;
                        else
                            break;
                }
            }

            return 66666;
        }

        /// <summary>
        /// 直线分析
        /// </summary>
        /// <param name=""></param>
        /// <param name="position"></param>
        /// <param name="GridNum"></param>
        /// <param name="StonePos"></param>
        /// <returns></returns>
        int AnalysisLine(int[] position, int GridNum, int StonePos)
        {
            Piece StoneType;
            int[] AnalyLine = new int[30];
            int nAnalyPos;
            int LeftEdge, RightEdge;
            int LeftRange, RightRange;
            if (GridNum < 5)
            {
                //数组长度小于5没有意义
                Array.Clear(m_LineRecord, (int) Pattern.ANALSISED, GridNum);
                return 0;
            }
            nAnalyPos = StonePos;
            Array.Clear(m_LineRecord, (int) Pattern.TOBEANALSIS, 30);
            Array.Clear(AnalyLine, 0x0F, 30);
            //将传入数组装入AnalyLine;
            Array.Copy(AnalyLine, position, GridNum);
            GridNum--;
            StoneType = (Piece) AnalyLine[nAnalyPos];
            LeftEdge = nAnalyPos;
            RightEdge = nAnalyPos;
            //算连续棋子左边界
            while (LeftEdge > 0)
            {
                if (AnalyLine[LeftEdge - 1] != (int) StoneType)
                    break;
                LeftEdge--;
            }

            //算连续棋子右边界
            while (RightEdge < GridNum)
            {
                if (AnalyLine[RightEdge + 1] != (int) StoneType)
                    break;
                RightEdge++;
            }
            LeftRange = LeftEdge;
            RightRange = RightEdge;
            //下面两个循环算出棋子可下的范围
            while (LeftRange > 0)
            {
                if (AnalyLine[LeftRange - 1] == (StoneType == Piece.White ? (int) Piece.Black : (int) Piece.White))
                    break;
                LeftRange--;
            }
            while (RightRange < GridNum)
            {
                if (AnalyLine[RightRange + 1] == (StoneType == Piece.White ? (int) Piece.Black : (int) Piece.White))
                    break;
                RightRange++;
            }
            //如果此范围小于4则分析没有意义
            if (RightRange - LeftRange < 4)
            {
                for (int k = LeftRange; k <= RightRange; k++)
                    m_LineRecord[k] = (int) Pattern.ANALSISED;
                return -1;
            }
            //将连续区域设为分析过的,防止重复分析此一区域
            for (int k = LeftEdge; k <= RightEdge; k++)
                m_LineRecord[k] = (int) Pattern.ANALSISED;
            if (RightEdge - LeftEdge > 3)
            {
                //如待分析棋子棋型为五连
                m_LineRecord[nAnalyPos] = (int) Pattern.Five;
                return (int) Pattern.Five;
            }

            if (RightEdge - LeftEdge == 3)
            {
                //如待分析棋子棋型为四连
                bool Leftfour = false;
                if (LeftEdge > 0)
                    if (AnalyLine[LeftEdge - 1] == (int) Piece.NULL)
                        Leftfour = true; //左边有气

                if (RightEdge < GridNum)
                    //右边未到边界
                    if (AnalyLine[RightEdge + 1] == (int) Piece.NULL)
                        //右边有气
                        if (Leftfour == true) //如左边有气
                            m_LineRecord[nAnalyPos] = (int) Pattern.Alive4; //活四
                        else
                            m_LineRecord[nAnalyPos] = (int) Pattern.Sleep4; //冲四
                    else if (Leftfour == true) //如左边有气
                        m_LineRecord[nAnalyPos] = (int) Pattern.Sleep4; //冲四
                    else if (Leftfour == true) //如左边有气
                        m_LineRecord[nAnalyPos] = (int) Pattern.Sleep4; //冲四

                return m_LineRecord[nAnalyPos];
            }

            if (RightEdge - LeftEdge == 2)
            {
                //如待分析棋子棋型为三连
                bool LeftThree = false;

                if (LeftEdge > 1)
                    if (AnalyLine[LeftEdge - 1] == (int) Piece.NULL)
                        //左边有气
                        if (LeftEdge > 1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
                        {
                            //左边隔一空白有己方棋子
                            m_LineRecord[LeftEdge] = (int) Pattern.Sleep4; //冲四
                            m_LineRecord[LeftEdge - 2] = (int) Pattern.ANALSISED;
                        }
                        else
                            LeftThree = true;

                if (RightEdge < GridNum)
                    if (AnalyLine[RightEdge + 1] == (int) Piece.NULL)
                        //右边有气
                        if (RightEdge < GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
                        {
                            //右边隔1个己方棋子
                            m_LineRecord[RightEdge] = (int) Pattern.Sleep4; //冲四
                            m_LineRecord[RightEdge + 2] = (int) Pattern.ANALSISED;
                        }
                        else if (LeftThree == true) //如左边有气
                            m_LineRecord[RightEdge] = (int) Pattern.Alive3; //活三
                        else
                            m_LineRecord[RightEdge] = (int) Pattern.Sleep3; //冲三
                    else
                    {
                        if (m_LineRecord[LeftEdge] == (int) Pattern.Sleep4) //如左冲四
                            return m_LineRecord[LeftEdge]; //返回

                        if (LeftThree == true) //如左边有气
                            m_LineRecord[nAnalyPos] = (int) Pattern.Sleep3; //眠三
                    }
                else
                {
                    if (m_LineRecord[LeftEdge] == (int) Pattern.Sleep4) //如左冲四
                        return m_LineRecord[LeftEdge]; //返回
                    if (LeftThree == true) //如左边有气
                        m_LineRecord[nAnalyPos] = (int) Pattern.Sleep3; //眠三
                }

                return m_LineRecord[nAnalyPos];
            }

            if (RightEdge - LeftEdge == 1)
            {
                //如待分析棋子棋型为二连
                bool Lefttwo = false;
                bool Leftthree = false;

                if (LeftEdge > 2)
                    if (AnalyLine[LeftEdge - 1] == (int) Piece.NULL)
                        //左边有气
                        if (LeftEdge - 1 > 1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
                            if (AnalyLine[LeftEdge - 3] == AnalyLine[LeftEdge])
                            {
                                //左边隔2个己方棋子
                                m_LineRecord[LeftEdge - 3] = (int) Pattern.ANALSISED;
                                m_LineRecord[LeftEdge - 2] = (int) Pattern.ANALSISED;
                                m_LineRecord[LeftEdge] = (int) Pattern.Sleep4; //冲四
                            }
                            else if (AnalyLine[LeftEdge - 3] == (int) Piece.NULL)
                            {
                                //左边隔1个己方棋子
                                m_LineRecord[LeftEdge - 2] = (int) Pattern.ANALSISED;
                                m_LineRecord[LeftEdge] = (int) Pattern.Sleep3; //眠三
                            }
                            else
                                Lefttwo = true;

                if (RightEdge < GridNum - 2)
                    if (AnalyLine[RightEdge + 1] == (int) Piece.NULL)
                        //右边有气
                        if (RightEdge + 1 < GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
                            if (AnalyLine[RightEdge + 3] == AnalyLine[RightEdge])
                            {
                                //右边隔两个己方棋子
                                m_LineRecord[RightEdge + 3] = (int) Pattern.ANALSISED;
                                m_LineRecord[RightEdge + 2] = (int) Pattern.ANALSISED;
                                m_LineRecord[RightEdge] = (int) Pattern.Sleep4; //冲四
                            }
                            else if (AnalyLine[RightEdge + 3] == (int) Piece.NULL)
                            {
                                //右边隔 1 个己方棋子
                                m_LineRecord[RightEdge + 2] = (int) Pattern.ANALSISED;
                                m_LineRecord[RightEdge] = (int) Pattern.ANALSISED; //眠三
                            }
                            else
                            {
                                if (m_LineRecord[LeftEdge] == (int) Pattern.Sleep4) //左边冲四
                                    return m_LineRecord[LeftEdge]; //返回

                                if (m_LineRecord[LeftEdge] == (int) Pattern.Sleep3) //左边眠三        
                                    return m_LineRecord[LeftEdge];

                                if (Lefttwo == true)
                                    m_LineRecord[nAnalyPos] = (int) Pattern.Alive2; //返回活二
                                else
                                    m_LineRecord[nAnalyPos] = (int) Pattern.Sleep2; //眠二
                            }
                        else
                        {
                            if (m_LineRecord[LeftEdge] == (int) Pattern.Sleep4) //冲四返回
                                return m_LineRecord[LeftEdge];

                            if (Lefttwo == true) //眠二
                                m_LineRecord[nAnalyPos] = (int) Pattern.Sleep2;
                        }

                return m_LineRecord[nAnalyPos];
            }

            return 0;
        }

        /// <summary>
        /// 分析棋盘上某点在水平方向上的棋型
        /// </summary>
        /// <param name=""></param>
        /// <param name="position"></param>
        /// <param name=""></param>
        /// <param name="i"></param>
        /// <param name="j"></param>
        /// <returns></returns>
        int AnalysisHorizon(int[,] position, int i, int j)
        {
            //将水平方向的棋子转入一维数组

            int[] tempArray = new int[GRID_NUM];
            //将垂直方向上的棋子转入一维数组
            for (int k = 0; k < GRID_NUM; k++)
                tempArray[k] = position[i, k];

            //调用直线分析函数分析
            AnalysisLine(tempArray, 15, j);
            //拾取分析结果
            for (int s = 0; s < 15; s++)
                if (m_LineRecord[s] != (int) Pattern.TOBEANALSIS)
                    TypeRecord[i, s, 0] = m_LineRecord[s];

            return TypeRecord[i, j, 0];
        }

        /// <summary>
        /// 分析棋盘上某点在垂直方向上的棋型
        /// </summary>
        /// <param name=""></param>
        /// <param name="position"></param>
        /// <param name=""></param>
        /// <param name="i"></param>
        /// <param name="j"></param>
        /// <returns></returns>
        int AnalysisVertical(int[,] position, int i, int j)
        {
            int[] tempArray = new int[GRID_NUM];
            //将垂直方向上的棋子转入一维数组
            for (int k = 0; k < GRID_NUM; k++)
                tempArray[k] = position[k, j];
            //调用直线分析函数分析
            AnalysisLine(tempArray, GRID_NUM, i);
            //拾取分析结果
            for (int s = 0; s < GRID_NUM; s++)
                if (m_LineRecord[s] != (int) Pattern.TOBEANALSIS)
                    TypeRecord[s, j, 1] = m_LineRecord[s];

            return TypeRecord[i, j, 1];
        }

        /// <summary>
        /// 分析棋盘上某点在左斜方向上的棋型
        /// </summary>
        /// <param name="position"></param>
        /// <param name="i"></param>
        /// <param name="j"></param>
        /// <returns></returns>
        int AnalysisLeft(int[,] position, int i, int j)
        {
            int[] tempArray = new int[GRID_NUM];
            int x, y;
            int k;
            if (i < j)
            {
                y = 0;
                x = j - i;
            }
            else
            {
                x = 0;
                y = i - j;
            }
            //将斜方向上的棋子转入一维数组
            for (k = 0; k < GRID_NUM; k++)
            {
                if (x + k > 14 || y + k > 14)
                    break;
                tempArray[k] = position[y + k, x + k];
            }
            //调用直线分析函数分析
            AnalysisLine(tempArray, k, j - x);
            //拾取分析结果
            for (int s = 0; s < k; s++)
                if (m_LineRecord[s] != (int) Pattern.TOBEANALSIS)
                    TypeRecord[y + s, x + s, 2] = m_LineRecord[s];

            return TypeRecord[i, j, 2];
        }

        /// <summary>
        /// 分析棋盘上某点在右斜方向上的棋型
        /// </summary>
        /// <param name="position"></param>
        /// <param name="i"></param>
        /// <param name="j"></param>
        /// <returns></returns>
        int AnalysisRight(int[,] position, int i, int j)
        {
            int[] tempArray = new int[GRID_NUM];
            int x, y, realnum;
            int k;
            if (14 - i < j)
            {
                y = 14;
                x = j - 14 + i;
                realnum = 14 - i;
            }
            else
            {
                x = 0;
                y = i + j;
                realnum = j;
            }
            //将斜方向上的棋子转入一维数组
            for (k = 0; k < GRID_NUM; k++)
            {
                if (x + k > 14 || y - k < 0)
                    break;
                tempArray[k] = position[y - k, x + k];
            }
            //调用直线分析函数分析
            AnalysisLine(tempArray, k, j - x);
            //拾取分析结果
            for (int s = 0; s < k; s++)
                if (m_LineRecord[s] != (int) Pattern.TOBEANALSIS)
                    TypeRecord[y - s, x + s, 3] = m_LineRecord[s];

            return TypeRecord[i, j, 3];
        }

        /// <summary>
        /// 估值函数
        /// </summary>
        /// <param name=""></param>
        /// <param name="position"></param>
        /// <param name=""></param>
        /// <param name="bIsWhiteTurn"></param>
        /// <returns></returns>
        int Eveluate(int[,] position, bool bIsWhiteTurn)
        {
            int i, j, k;
            int nStoneType;
            count++; //计数器累加

            //清空棋型分析结果
            Array.Clear(TypeRecord, (int) Pattern.TOBEANALSIS, GRID_COUNT * 4 * 4);
            Array.Clear(TypeCount, 0, 40 * 4);

            for (i = 0; i < GRID_NUM; i++)
            for (j = 0; j < GRID_NUM; j++)
            {
                if (position[i, j] != (int) (Piece.NULL))
                {
                    //如果水平方向上没有分析过
                    if (TypeRecord[i, j, 0] == (int) Pattern.TOBEANALSIS)
                        AnalysisHorizon(position, i, j);

                    //如果垂直方向上没有分析过
                    if (TypeRecord[i, j, 1] == (int) Pattern.TOBEANALSIS)
                        AnalysisVertical(position, i, j);

                    //如果左斜方向上没有分析过
                    if (TypeRecord[i, j, 2] == (int) Pattern.TOBEANALSIS)
                        AnalysisLeft(position, i, j);

                    //如果右斜方向上没有分析过
                    if (TypeRecord[i, j, 3] == (int) Pattern.TOBEANALSIS)
                        AnalysisRight(position, i, j);
                }
            }

            //对分析结果进行统计,得到每种棋型的数量
            for (i = 0; i < GRID_NUM; i++)
            for (j = 0; j < GRID_NUM; j++)
            for (k = 0; k < 4; k++)
            {
                nStoneType = position[i, j];
                if (nStoneType != (int) Piece.NULL)
                {
                    switch (TypeRecord[i, j, k])
                    {
                        case (int) Pattern.Five: //五连
                            TypeCount[nStoneType, (int) Pattern.Five]++;
                            break;
                        case (int) Pattern.Alive4: //活四
                            TypeCount[nStoneType, (int) Pattern.Alive4]++;
                            break;
                        case (int) Pattern.Sleep4: //眠四
                            TypeCount[nStoneType, (int) Pattern.Sleep4]++;
                            break;
                        case (int) Pattern.Alive3: //活三
                            TypeCount[nStoneType, (int) Pattern.Alive3]++;
                            break;
                        case (int) Pattern.Sleep3: //眠三
                            TypeCount[nStoneType, (int) Pattern.Sleep3]++;
                            break;
                        case (int) Pattern.Alive2: //活二
                            TypeCount[nStoneType, (int) Pattern.Alive2]++;
                            break;
                        case (int) Pattern.Sleep2: //眠二
                            TypeCount[nStoneType, (int) Pattern.Sleep2]++;
                            break;
                        default:
                            break;
                    }
                }
            }

            //如果已五连,返回极值
            if (bIsWhiteTurn)
            {
                if (TypeCount[(int) Piece.Black, (int) Pattern.Five] >= 1)
                {
                    return -9999;
                }
                if (TypeCount[(int) Piece.White, (int) Pattern.Five] >= 1)
                {
                    return 9999;
                }
            }
            else
            {
                if (TypeCount[(int) Piece.Black, (int) Pattern.Five] >= 1)
                {
                    return 9999;
                }
                if (TypeCount[(int) Piece.White, (int) Pattern.Five] >= 1)
                {
                    return -9999;
                }
            }
            //两个冲四等于一个活四
            if (TypeCount[(int) Piece.White, (int) Pattern.Sleep4] > 1)
                TypeCount[(int) Piece.White, (int) Pattern.Alive4]++;
            if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep4] > 1)
                TypeCount[(int) Piece.Black, (int) Pattern.Alive4]++;
            int WValue = 0, BValue = 0;

            if (bIsWhiteTurn) //轮到白棋走
            {
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive4] >= 1)
                {
                    return 9990; //活四,白胜返回极值
                }
                if (TypeCount[(int) Piece.White, (int) Pattern.Sleep4] >= 1)
                {


                    return 9980; //冲四,白胜返回极值
                }
                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive4] >= 1)
                {


                    return -9970; //白无冲四活四,而黑有活四,黑胜返回极值
                }
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep4] >= 1 &&
                    TypeCount[(int) Piece.Black, (int) Pattern.Alive3] >= 1)
                {


                    return -9960; //而黑有冲四活三,黑胜返回极值
                }
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive3] >= 1 &&
                    TypeCount[(int) Piece.Black, (int) Pattern.Sleep4] == 0)
                {


                    return 9950; //白有活三而黑没有四,白胜返回极值
                }
                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive3] > 1 &&
                    TypeCount[(int) Piece.White, (int) Pattern.Sleep4] == 0 &&
                    TypeCount[(int) Piece.White, (int) Pattern.Alive3] == 0 &&
                    TypeCount[(int) Piece.White, (int) Pattern.Sleep3] == 0)
                {


                    return -9940; //黑的活三多于一个,而白无四和三,黑胜返回极值
                }
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive3] > 1)
                    WValue += 2000; //白活三多于一个,白棋价值加2000
                else
                    //否则白棋价值加200
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive3] >= 1)
                    WValue += 200;
                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive3] > 1)
                    BValue += 500; //黑的活三多于一个,黑棋价值加500
                else
                    //否则黑棋价值加100
                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive3] >= 1)
                    BValue += 100;
                //每个眠三加10
                if (TypeCount[(int) Piece.White, (int) Pattern.Sleep3] >= 1)
                    WValue += TypeCount[(int) Piece.White, (int) Pattern.Sleep3] * 10;
                //每个眠三加10
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep3] >= 1)
                    BValue += TypeCount[(int) Piece.Black, (int) Pattern.Sleep3] * 10;
                //每个活二加4
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive2] >= 1)
                    WValue += TypeCount[(int) Piece.White, (int) Pattern.Alive2] * 4;
                //每个活二加4
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep2] >= 1)
                    BValue += TypeCount[(int) Piece.Black, (int) Pattern.Alive2] * 4;
                //每个眠二加1
                if (TypeCount[(int) Piece.White, (int) Pattern.Sleep2] >= 1)
                    WValue += TypeCount[(int) Piece.White, (int) Pattern.Sleep2];
                //每个眠二加1
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep2] >= 1)
                    BValue += TypeCount[(int) Piece.Black, (int) Pattern.Sleep2];
            }
            else //轮到黑棋走
            {
                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive4] >= 1)
                {

                    return 9990; //活四,黑胜返回极值
                }
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep4] >= 1)
                {

                    return 9980; //冲四,黑胜返回极值
                }
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive4] >= 1)
                    return -9970; //活四,白胜返回极值

                if (TypeCount[(int) Piece.White, (int) Pattern.Sleep4] >= 1 &&
                    TypeCount[(int) Piece.White, (int) Pattern.Alive3] >= 1)
                    return -9960; //冲四并活三,白胜返回极值

                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive3] >= 1 &&
                    TypeCount[(int) Piece.White, (int) Pattern.Sleep4] == 0)
                    return 9950; //黑活三,白无四。黑胜返回极值

                if (TypeCount[(int) Piece.White, (int) Pattern.Alive3] > 1 &&
                    TypeCount[(int) Piece.Black, (int) Pattern.Sleep4] == 0 &&
                    TypeCount[(int) Piece.Black, (int) Pattern.Alive3] == 0 &&
                    TypeCount[(int) Piece.Black, (int) Pattern.Sleep3] == 0)
                    return -9940; //白的活三多于一个,而黑无四和三,白胜返回极值

                //黑的活三多于一个,黑棋价值加2000
                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive3] > 1)
                    BValue += 2000;
                else
                    //否则黑棋价值加200
                if (TypeCount[(int) Piece.Black, (int) Pattern.Alive3] >= 1)
                    BValue += 200;

                //白的活三多于一个,白棋价值加 500
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive3] > 1)
                    WValue += 500;
                else
                    //否则白棋价值加100
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive3] >= 1)
                    WValue += 100;

                //每个眠三加10
                if (TypeCount[(int) Piece.White, (int) Pattern.Sleep3] >= 1)
                    WValue += TypeCount[(int) Piece.White, (int) Pattern.Sleep3] * 10;
                //每个眠三加10
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep3] >= 1)
                    BValue += TypeCount[(int) Piece.Black, (int) Pattern.Sleep3] * 10;

                //每个活二加4
                if (TypeCount[(int) Piece.White, (int) Pattern.Alive2] >= 1)
                    WValue += TypeCount[(int) Piece.White, (int) Pattern.Alive2] * 4;
                //每个活二加4
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep2] >= 1)
                    BValue += TypeCount[(int) Piece.Black, (int) Pattern.Alive2] * 4;

                //每个眠二加1
                if (TypeCount[(int) Piece.White, (int) Pattern.Sleep2] >= 1)
                    WValue += TypeCount[(int) Piece.White, (int) Pattern.Sleep2];
                //每个眠二加1
                if (TypeCount[(int) Piece.Black, (int) Pattern.Sleep2] >= 1)
                    BValue += TypeCount[(int) Piece.Black, (int) Pattern.Sleep2];
            }

            //加上所有棋子的位置价值
            for (i = 0; i < GRID_NUM; i++)
            for (j = 0; j < GRID_NUM; j++)
            {
                nStoneType = position[i, j];
                if (nStoneType != (int) Piece.NULL)
                    if (nStoneType == (int) Piece.Black)
                        BValue += PosValue[i, j];
                    else
                        WValue += PosValue[i, j];
            }

            //返回估值
            if (!bIsWhiteTurn)
                return BValue - WValue;
            else
                return WValue - BValue;
        }

        /// <summary>
        /// 进入置换表
        /// </summary>
        /// <param name="entry_type"></param>
        /// <param name="eval"></param>
        /// <param name="depth"></param>
        /// <param name="TableNo"></param>
        void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo)
        {
            int x;
            HashItem pht;

            x = (int) (m_HashKey32 & 0xFFFFF); //计算二十位哈希地址
            pht = m_pTT[TableNo][x]; //取到具体的表项指针

            //将数据写入哈希表
            pht.checksum = m_HashKey64; //64位校验码
            pht.entry_type = entry_type; //表项类型
            pht.eval = eval; //要保存的值
            pht.depth = depth; //层次
        }

        /// <summary>
        /// 在m_MoveList中插入一个走法
        /// </summary>
        /// <param name="nToX">目标位置横坐标</param>
        /// <param name="nToY">目标位置纵坐标</param>
        /// <param name="nPly">是此走法所在的层次</param>
        /// <returns></returns>
        int AddMove(int nToX, int nToY, int nPly)
        {
            m_MoveList[nPly, m_nMoveCount].StonePos.x = nToX;
            m_MoveList[nPly, m_nMoveCount].StonePos.y = nToY;
            m_nMoveCount++;
            m_MoveList[nPly, m_nMoveCount].Score = PosValue[nToY, nToX]; //使用位置价值表评估当前走法的价值
            return m_nMoveCount;
        }

        /// <summary>
        /// 对走法队列从小到大排序
        /// 合并source[l…m]和 source[m +1…r]至target[l…r]
        /// </summary>
        /// <param name="source">原始队列</param>
        /// <param name="target">目标队列</param>
        /// <param name="l"></param>
        /// <param name="m"></param>
        /// <param name="r"></param>
        void Merge(STONEMOVE[] source, STONEMOVE[] target, int l, int m, int r)
        {
            //从小到大排序
            int i = l;
            int j = m + 1;
            int k = l;
            while (i <= m && j <= r)
                if (source[i].Score <= source[j].Score)
                    target[k++] = source[i++];
                else
                    target[k++] = source[j++];
            if (i > m)
                for (int q = j; q <= r; q++)
                    target[k++] = source[q];
            else
                for (int q = i; q <= m; q++)
                    target[k++] = source[q];
        }

        /// <summary>
        /// 从大到小排序
        /// </summary>
        /// <param name="source"></param>
        /// <param name="target"></param>
        /// <param name="l"></param>
        /// <param name="m"></param>
        /// <param name="r"></param>
        void Merge_A(STONEMOVE[] source, STONEMOVE[] target, int l, int m, int r)
        {
            //
            int i = l;
            int j = m + 1;
            int k = l;
            while (i <= m && j <= r)
                if (source[i].Score >= source[j].Score)
                    target[k++] = source[i++];
                else
                    target[k++] = source[j++];
            if (i > m)
                for (int q = j; q <= r; q++)
                    target[k++] = source[q];
            else
                for (int q = i; q <= m; q++)
                    target[k++] = source[q];
        }

        /// <summary>
        /// 合并大小为 S 的相邻子数组
        /// </summary>
        /// <param name="source"></param>
        /// <param name="target"></param>
        /// <param name="s"></param>
        /// <param name="n"></param>
        /// <param name="direction">从大到小还是从小到大排序</param>
        void MergePass(STONEMOVE[] source, STONEMOVE[] target, int s, int n, bool direction)
        {

            int i = 0;
            while (i <= n - 2 * s)
            {
                //合并大小为 s的相邻二段子数组
                if (direction)
                    Merge(source, target, i, i + s - 1, i + 2 * s - 1);
                else

                    Merge_A(source, target, i, i + s - 1, i + 2 * s - 1);
                i = i + 2 * s;
            }
            if (i + s < n) //剩余的元素个数小于2s
            {
                if (direction)
                    Merge(source, target, i, i + s - 1, n - 1);
                else

                    Merge_A(source, target, i, i + s - 1, n - 1);
            }
            else
                for (int j = i; j <= n - 1; j++)
                    target[j] = source[j];
        }

        /// <summary>
        /// 归并排序,把探索价值高的点排在前面,按照Score排列待探索点
        /// </summary>
        /// <param name="source"></param>
        /// <param name="n"></param>
        /// <param name="direction"></param>
        /// 
        void MergeSort(STONEMOVE[] source, int n, bool direction)
        {
            int s = 1;
            while (s < n)
            {
                MergePass(source, m_TargetBuff, s, n, direction);
                s += s;
                MergePass(m_TargetBuff, source, s, n, direction);
                s += s;
            }
        }

        /// <summary>
        /// 生成待探索落点
        /// 整个棋盘上所有没落子的点都是待探索的,按照位置权值排序,优先探索中间的
        /// </summary>
        /// <param name=""></param>
        /// <param name="position"></param>
        /// <param name=""></param>
        /// <param name="nPly"></param>
        /// <param name="nSide"></param>
        /// <returns></returns>
        int CreatePossibleMove(int[,] position, int nPly, int nSide)
        {
            int i, j;
            m_nMoveCount = 0;
            for (i = 0; i < GRID_NUM; i++)
            for (j = 0; j < GRID_NUM; j++)
            {
                if (position[i, j] == (int) Piece.NULL)
                    AddMove(j, i, nPly);
            }
            //二维数组提取一行
            STONEMOVE[] temp_m_MoveList = new STONEMOVE[GRID_COUNT];
            for (int k = 0; k < GRID_COUNT; k++)
            {
                temp_m_MoveList[k] = m_MoveList[nPly, k];
            }
            MergeSort(temp_m_MoveList, m_nMoveCount, false);
            return m_nMoveCount; //返回合法走法个数
        }

        /// <summary>
        /// 从历史得分表中取给定走法的历史得分
        /// </summary>
        /// <param name="move"></param>
        /// <returns></returns>
        int GetHistoryScore(STONEMOVE move)
        {
            return m_HistoryTable[move.StonePos.x, move.StonePos.y];
        }

        /// <summary>
        /// 将一最佳走法汇入历史记录
        /// </summary>
        /// <param name="move"></param>
        /// <param name="depth"></param>
        void EnterHistoryScore(STONEMOVE move, int depth)
        {
            m_HistoryTable[move.StonePos.x, move.StonePos.y] += 2 << depth;
        }

        /// <summary>
        /// 将历史记录表中所有项目全置为初值
        /// </summary>
        void ResetHistoryTable()
        {
            //Array.Clear(m_HistoryTable, 10, GRID_COUNT /** sizeof(int)*/);
            for (int i = 0; i < GRID_NUM; i++)
            {
                for (int j = 0; j < GRID_NUM; j++)
                {
                    m_HistoryTable[i, j] = 10;
                }
            }
        }

        /// <summary>
        /// 让type所指的一方做move中的落子动作
        /// </summary>
        /// <param name="move">动作</param>
        /// <param name="type">黑/白</param>
        /// <returns>没啥用</returns>
        int MakeMove(STONEMOVE move, int type)
        {
            CurPosition[move.StonePos.y, move.StonePos.x] = type;
            return 0;
        }

        /// <summary>
        /// 撤销move这一步动作,不管他是哪一方的棋子
        /// </summary>
        /// <param name="move"></param>
        void UnMakeMove(STONEMOVE move)
        {
            CurPosition[move.StonePos.y, move.StonePos.x] = (int) Piece.NULL;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="move"></param>
        /// <param name="CurPosition"></param>
        void Hash_MakeMove(STONEMOVE move, int[,] CurPosition)
        {
            int type;

            type = CurPosition[move.StonePos.y, move.StonePos.x]; //将棋子在目标位置的随机数添入
            m_HashKey32 = m_HashKey32 ^ (uint) m_nHashKey32[type, move.StonePos.y, move.StonePos.x];
            m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type, move.StonePos.y, move.StonePos.x];
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="move"></param>
        /// <param name="CurPosition"></param>
        void Hash_UnMakeMove(STONEMOVE move, int[,] CurPosition)
        {
            int type;
            type = CurPosition[move.StonePos.y, move.StonePos.x]; //将棋子现在位置上的随机数从哈希值当中去除
            m_HashKey32 = m_HashKey32 ^ (uint) m_nHashKey32[type, move.StonePos.y, move.StonePos.x];
            m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type, move.StonePos.y, move.StonePos.x];
        }

        /// <summary>
        /// 生成64位整形随机数
        /// </summary>
        /// <returns></returns>
        Int64 rand64()
        {
            return m_randomGenerator.Next(32767) ^ ((Int64) m_randomGenerator.Next(32767) << 15) ^
                   ((Int64) m_randomGenerator.Next(32767) << 30) ^ ((Int64) m_randomGenerator.Next(32767) << 45) ^
                   ((Int64) m_randomGenerator.Next(32767) << 60);
        }

        /// <summary>
        /// 生成32位随机数
        /// </summary>
        /// <returns></returns>
        long rand32()
        {
            return m_randomGenerator.Next(32767) ^ ((long) m_randomGenerator.Next(32767) << 15) ^
                   ((long) m_randomGenerator.Next(32767) << 30);
        }

        /// <summary>
        /// 哈希KEY初始化
        /// </summary>
        void InitializeHashKey()
        {
            int i, j, k;
            //填充随机数组
            for (i = 0; i < 15; i++)
            for (j = 0; j < 10; j++)
            for (k = 0; k < 9; k++)
            {
                m_nHashKey32[i, j, k] = (int) rand32();
                m_ulHashKey64[i, j, k] = rand64();
            }

            //申请置换表所用空间。1M "2 个条目，读者也可指定其他大小
            m_pTT[0] = new List<HashItem>(1024 * 1024); //用于存放取极大值的节点数据
            m_pTT[1] = new List<HashItem>(1024 * 1024); //用于存放取极小值的节点数据
        }

        /// <summary>
        /// 将所有棋子对应的哈希数加总
        /// </summary>
        /// <param name=""></param>
        /// <param name="CurPosition"></param>
        /// <param name=""></param>
        void CalculateInitHashKey(int[,] CurPosition)
        {
            int j, k, nStoneType;
            m_HashKey32 = 0;
            m_HashKey32 = 0;
            //将所有棋子对应的哈希数加总
            for (j = 0; j < GRID_NUM; j++)
            for (k = 0; k < GRID_NUM; k++)
            {
                nStoneType = CurPosition[j, k];
                if (nStoneType != 0xFF)
                {
                    m_HashKey32 = (uint) (m_HashKey32 ^ m_nHashKey32[nStoneType, j, k]);
                    m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nStoneType, j, k];
                }
            }
        }

        /// <summary>
        /// 释放哈希表所用空间
        /// </summary>
        void _CTranspositionTable()
        {
            m_pTT[0].Clear();
            m_pTT[1].Clear();
        }

        /// <summary>
        /// 使用魔法检索出一个最优动作
        /// </summary>
        /// <param name=""></param>
        /// <param name="position">对方已经落子之后的残局</param>
        /// <param name="Type">棋子颜色</param>
        void SearchAGoodMove(int[,] position, int Type)
        {
            int Score;
            Array.Copy(CurPosition, position, GRID_COUNT);//制造当前残局的副本便于分析
            m_nMaxDepth = m_nSearchDepth;
            CalculateInitHashKey(CurPosition);
            ResetHistoryTable();
            Score = NegaScout(m_nMaxDepth, -20000, 20000);
            X = m_cmBestMove.StonePos.y;
            Y = m_cmBestMove.StonePos.x;
            MakeMove(m_cmBestMove, Type);
            Array.Copy(position, CurPosition, GRID_COUNT);
        }

        /// <summary>
        /// 负极大值算法
        /// </summary>
        /// <param name="depth"></param>
        /// <param name="alpha"></param>
        /// <param name="beta"></param>
        /// <returns></returns>
        int NegaScout(int depth, int alpha, int beta)
        {
            int Count, i;
            int type;
            int a, b, t;
            int side;
            int score;
            /*        if(depth>0)
            {
            i= IsGameOver(CurPosition,depth);
            if(i!=0)
            return i;  //已分胜负，返回极值
            }
            */
            side = (m_nMaxDepth - depth) % 2; //计算当前节点的类型,极大0/极小1
            score = LookUpHashTable(alpha, beta, depth, side);
            if (score != 66666)
                return score;
            if (depth <= 0) //叶子节点取估值
            {
                score = Eveluate(CurPosition, (side != 0));
                EnterHashTable(ENTRY_TYPE.exact, (short) score, (short) depth, (short) side); //将估值存入置换表

                return score;
            }
            Count = CreatePossibleMove(CurPosition, depth, side);
            for (i = 0; i < Count; i++)
                m_MoveList[depth, i].Score = GetHistoryScore(m_MoveList[depth, i]);
            //二维数组提取一维
            STONEMOVE[] temp_m_MoveList = new STONEMOVE[GRID_COUNT];
            for (int j = 0; j < GRID_COUNT; j++)
            {
                temp_m_MoveList[j] = m_MoveList[depth, j];
            }
            MergeSort(temp_m_MoveList, Count, false);
            int bestmove = -1;
            a = alpha;
            b = beta;

            int eval_is_exact = 0;

            for (i = 0; i < Count; i++)
            {
                type = MakeMove(m_MoveList[depth, i], side);
                Hash_MakeMove(m_MoveList[depth, i], CurPosition);
                t = -NegaScout(depth - 1, -b, -a); //递归搜索子节点，对第 1 个节点是全窗口，其后是空窗探测
                if (t > a && t < beta && i > 0)
                {
                    //对于第一个后的节点,如果上面的搜索failhigh
                    a = -NegaScout(depth - 1, -beta, -t); //re-search
                    eval_is_exact = 1; //设数据类型为精确值
                    if (depth == m_nMaxDepth)
                        m_cmBestMove = m_MoveList[depth, i];
                    bestmove = i;
                }
                Hash_UnMakeMove(m_MoveList[depth, i], CurPosition);
                UnMakeMove(m_MoveList[depth, i]);
                if (a < t)
                {
                    eval_is_exact = 1;
                    a = t;
                    if (depth == m_nMaxDepth)
                        m_cmBestMove = m_MoveList[depth, i];
                }
                if (a >= beta)
                {
                    EnterHashTable(ENTRY_TYPE.lower_bound, (short) a, (short) depth, side);
                    EnterHistoryScore(m_MoveList[depth, i], depth);
                    return a;
                }
                b = a + 1; /* set new null window */
            }
            if (bestmove != -1)
                EnterHistoryScore(m_MoveList[depth, bestmove], depth);
            if (eval_is_exact == 1)
                EnterHashTable(ENTRY_TYPE.exact, (short) a, (short) depth, side);
            else
                EnterHashTable(ENTRY_TYPE.upper_bound, (short) a, (short) depth, side);

            return a;
        }

        /// <summary>
        /// α-β剪枝算法初始化
        /// 变量初始化,设置人类玩家和AI玩家所选的棋子的颜色
        /// 目前先考虑让人类玩家执黑先行
        /// </summary>
        public void AlphaBetaInit()
        {
            for (int i = 0; i < GRID_NUM; i++)
            for (int j = 0; j < GRID_NUM; j++)
                m_RenjuBoard[i, j] = (int) Piece.NULL; //棋盘初始化 
            colour = (int) Piece.White;
            m_nUserStoneColor = (int) Piece.Black; //人类执黑先行

        }

        /// <summary>
        /// 获取AI的下一步行动
        /// 如果AI是执黑先行,则人类玩家落子坐标传入-1,-1
        /// </summary>
        /// <param name="playerX">人类玩家落子坐标X</param>
        /// <param name="playerY">人类玩家落子坐标Y</param>
        /// <param name="finalX">AI落子坐标X</param>
        /// <param name="finalY">AI落子坐标Y</param>
        public void getComputerAction(int playerX, int playerY, out int finalX, out int finalY)
        {

            if (colour == (int) Piece.Black) //如果己方执黑先行，则占据棋盘中心位置 
            {
                m_RenjuBoard[GRID_NUM / 2, GRID_NUM / 2] = colour; //更新棋盘信息 
                finalX = GRID_NUM / 2;
                finalY = GRID_NUM / 2;
            }
            else
            {
                m_RenjuBoard[playerX, playerY] = m_nUserStoneColor;

                ResetHistoryTable();
                InitializeHashKey(); //建立哈希表，创建随机数组
                SearchAGoodMove(m_RenjuBoard, colour);
                m_RenjuBoard[X, Y] = colour;
                finalX = X;
                finalY = Y;
                _CTranspositionTable();
            }
        }


    }
}
