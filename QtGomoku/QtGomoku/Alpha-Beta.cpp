#include <cstdlib>
#include <iostream>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"StaticDefinition.h"
#include"Alpha-Beta.h"

#include <qDebug>
/*
* 估值函数
* 参数1:位置
* 参数2:true=白棋,false=黑棋
*/
int AiAgent::Eveluate(unsigned char position[][GRID_NUM], bool bIsWhiteTurn)
{
	int i, j, k;
	unsigned char nStoneType;
	count++;//计数器累加
			/*
			* void *memset(void *s, int ch, size_t n)
			* 函数解释：将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 。
			* memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法
			*/
			//清空棋型分析结果
	memset(TypeRecord, TOBEANALSIS, GRID_COUNT * 4 * 4);
	memset(TypeCount, 0, 40 * 4);

	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] != space)//当前分析的位置有棋子
			{
				//如果水平方向上没有分析过
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//如果垂直方向上没有分析过
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//如果左斜方向上没有分析过
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisLeft(position, i, j);

				//如果右斜方向上没有分析过
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisRight(position, i, j);
			}
		}

	//对分析结果进行统计,得到每种棋型的数量
	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
			for (k = 0; k<4; k++)
			{
				nStoneType = position[i][j];
				if (nStoneType != space)
				{
					switch (TypeRecord[i][j][k])
					{
					case FIVE://五连
						TypeCount[nStoneType][FIVE]++;
						break;
					case FOUR://活四
						TypeCount[nStoneType][FOUR]++;
						break;
					case SFOUR://冲四
						TypeCount[nStoneType][SFOUR]++;
						break;
					case THREE://活三
						TypeCount[nStoneType][THREE]++;
						break;
					case STHREE://眠三
						TypeCount[nStoneType][STHREE]++;
						break;
					case TWO://活二
						TypeCount[nStoneType][TWO]++;
						break;
					case STWO://眠二
						TypeCount[nStoneType][STWO]++;
						break;
					default:
						break;
					}
				}
			}

	//如果已五连,返回极值
	if (bIsWhiteTurn)
	{
		if (TypeCount[black][FIVE])
		{
			return -9999;
		}
		if (TypeCount[white][FIVE])
		{
			return 9999;
		}
	}
	else
	{
		if (TypeCount[black][FIVE])
		{
			return 9999;
		}
		if (TypeCount[white][FIVE])
		{
			return -9999;
		}
	}
	//两个冲四等于一个活四
	if (TypeCount[white][SFOUR]>1)
		TypeCount[white][FOUR]++;
	if (TypeCount[black][SFOUR]>1)
		TypeCount[black][FOUR]++;
	int WValue = 0, BValue = 0;

	if (bIsWhiteTurn)//轮到白棋走
	{
		if (TypeCount[white][FOUR])
		{


			return 9990;//活四,白胜返回极值
		}
		if (TypeCount[white][SFOUR])
		{


			return 9980;//冲四,白胜返回极值
		}
		if (TypeCount[black][FOUR])
		{


			return -9970;//白无冲四活四,而黑有活四,黑胜返回极值
		}
		if (TypeCount[black][SFOUR] && TypeCount[black][THREE])
		{


			return -9960;//而黑有冲四和活三,黑胜返回极值
		}
		if (TypeCount[white][THREE] && TypeCount[black][SFOUR] == 0)
		{


			return 9950;//白有活三而黑没有四,白胜返回极值
		}
		if (TypeCount[black][THREE]>1 && TypeCount[white][SFOUR] == 0 && TypeCount[white][THREE] == 0 && TypeCount[white][STHREE] == 0)
		{


			return -9940;//黑的活三多于一个,而白无四和三,黑胜返回极值
		}
		if (TypeCount[white][THREE]>1)
			WValue += 2000;//白活三多于一个,白棋价值加2000
		else
			//否则白棋价值加200
			if (TypeCount[white][THREE])
				WValue += 200;
		if (TypeCount[black][THREE]>1)
			BValue += 500;//黑的活三多于一个,黑棋价值加500
		else
			//否则黑棋价值加100
			if (TypeCount[black][THREE])
				BValue += 100;
		//每个眠三加10
		if (TypeCount[white][STHREE])
			WValue += TypeCount[white][STHREE] * 10;
		//每个眠三加10
		if (TypeCount[black][STHREE])
			BValue += TypeCount[black][STHREE] * 10;
		//每个活二加4
		if (TypeCount[white][TWO])
			WValue += TypeCount[white][TWO] * 4;
		//每个活二加4
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][TWO] * 4;
		//每个眠二加1
		if (TypeCount[white][STWO])
			WValue += TypeCount[white][STWO];
		//每个眠二加1
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][STWO];
	}
	else//轮到黑棋走
	{
		if (TypeCount[black][FOUR])
		{

			return 9990;//活四,黑胜返回极值
		}
		if (TypeCount[black][SFOUR])
		{

			return 9980;//冲四,黑胜返回极值
		}
		if (TypeCount[white][FOUR])
			return -9970;//活四,白胜返回极值

		if (TypeCount[white][SFOUR] && TypeCount[white][THREE])
			return -9960;//冲四并活三,白胜返回极值

		if (TypeCount[black][THREE] && TypeCount[white][SFOUR] == 0)
			return 9950;//黑活三,白无四。黑胜返回极值

		if (TypeCount[white][THREE]>1 && TypeCount[black][SFOUR] == 0 && TypeCount[black][THREE] == 0 && TypeCount[black][STHREE] == 0)
			return -9940;//白的活三多于一个,而黑无四和三,白胜返回极值

						 //黑的活三多于一个,黑棋价值加2000
		if (TypeCount[black][THREE]>1)
			BValue += 2000;
		else
			//否则黑棋价值加200
			if (TypeCount[black][THREE])
				BValue += 200;

		//白的活三多于一个,白棋价值加 500
		if (TypeCount[white][THREE]>1)
			WValue += 500;
		else
			//否则白棋价值加100
			if (TypeCount[white][THREE])
				WValue += 100;

		//每个眠三加10
		if (TypeCount[white][STHREE])
			WValue += TypeCount[white][STHREE] * 10;
		//每个眠三加10
		if (TypeCount[black][STHREE])
			BValue += TypeCount[black][STHREE] * 10;

		//每个活二加4
		if (TypeCount[white][TWO])
			WValue += TypeCount[white][TWO] * 4;
		//每个活二加4
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][TWO] * 4;

		//每个眠二加1
		if (TypeCount[white][STWO])
			WValue += TypeCount[white][STWO];
		//每个眠二加1
		if (TypeCount[black][STWO])
			BValue += TypeCount[black][STWO];
	}

	//加上所有棋子的位置价值
	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
		{
			nStoneType = position[i][j];
			if (nStoneType != space)
				if (nStoneType == black)
					BValue += PosValue[i][j];
				else
					WValue += PosValue[i][j];
		}

	//返回估值
	if (!bIsWhiteTurn)
		return BValue - WValue;
	else
		return WValue - BValue;
}
/*
 *分析棋盘上某点在水平方向上的棋型
 */
int AiAgent::AnalysisHorizon(unsigned char position[][GRID_NUM], int i, int j)
{
	//调用直线分析函数分析
	AnalysisLine(position[i], 15, j);
	//拾取分析结果
	for (int s = 0; s<15; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[i][s][0] = m_LineRecord[s];

	return TypeRecord[i][j][0];
}
/*
 *分析棋盘上某点在垂直方向上的棋型
 */
int AiAgent::AnalysisVertical(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	//将垂直方向上的棋子转入一维数组
	for (int k = 0; k<GRID_NUM; k++)
		tempArray[k] = position[k][j];
	//调用直线分析函数分析
	AnalysisLine(tempArray, GRID_NUM, i);
	//拾取分析结果
	for (int s = 0; s<GRID_NUM; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[s][j][1] = m_LineRecord[s];

	return TypeRecord[i][j][1];
}
/*
 *分析棋盘上某点在左斜方向上的棋型
 */
int AiAgent::AnalysisLeft(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	int x, y;
	int k;
	if (i<j)
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
	for (k = 0; k<GRID_NUM; k++)
	{
		if (x + k>14 || y + k>14)
			break;
		tempArray[k] = position[y + k][x + k];
	}
	//调用直线分析函数分析
	AnalysisLine(tempArray, k, j - x);
	//拾取分析结果
	for (int s = 0; s<k; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y + s][x + s][2] = m_LineRecord[s];

	return TypeRecord[i][j][2];
}
/*
 *分析棋盘上某点在右斜方向上的棋型
 */
int AiAgent::AnalysisRight(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	int x, y, realnum;
	int k;
	if (14 - i<j)
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
	for (k = 0; k<GRID_NUM; k++)
	{
		if (x + k>14 || y - k<0)
			break;
		tempArray[k] = position[y - k][x + k];
	}
	//调用直线分析函数分析
	AnalysisLine(tempArray, k, j - x);
	//拾取分析结果
	for (int s = 0; s<k; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y - s][x + s][3] = m_LineRecord[s];

	return TypeRecord[i][j][3];
}
/*
 *直线分析
 */
int AiAgent::AnalysisLine(unsigned char* position, int GridNum, int StonePos)
{
	unsigned char StoneType;
	unsigned char AnalyLine[30];
	int nAnalyPos;
	int LeftEdge, RightEdge;
	int LeftRange, RightRange;

	if (GridNum<5)
	{
		//数组长度小于5没有意义
		memset(m_LineRecord, ANALSISED, GridNum);
		return 0;
	}
	nAnalyPos = StonePos;
	memset(m_LineRecord, TOBEANALSIS, 30);
	memset(AnalyLine, 0x0F, 30);
	//将传入数组装入AnalyLine;
	memcpy(&AnalyLine, position, GridNum);
	GridNum--;
	StoneType = AnalyLine[nAnalyPos];
	LeftEdge = nAnalyPos;
	RightEdge = nAnalyPos;
	//算连续棋子左边界
	while (LeftEdge>0)
	{
		if (AnalyLine[LeftEdge - 1] != StoneType)
			break;
		LeftEdge--;
	}

	//算连续棋子右边界
	while (RightEdge<GridNum)
	{
		if (AnalyLine[RightEdge + 1] != StoneType)
			break;
		RightEdge++;
	}
	LeftRange = LeftEdge;
	RightRange = RightEdge;
	//下面两个循环算出棋子可下的范围
	while (LeftRange>0)
	{
		if (AnalyLine[LeftRange - 1] == !StoneType)
			break;
		LeftRange--;
	}
	while (RightRange<GridNum)
	{
		if (AnalyLine[RightRange + 1] == !StoneType)
			break;
		RightRange++;
	}
	//如果此范围小于4则分析没有意义
	if (RightRange - LeftRange<4)
	{
		for (int k = LeftRange; k <= RightRange; k++)
			m_LineRecord[k] = ANALSISED;
		return false;
	}
	//将连续区域设为分析过的,防止重复分析此一区域
	for (int k = LeftEdge; k <= RightEdge; k++)
		m_LineRecord[k] = ANALSISED;
	if (RightEdge - LeftEdge>3)
	{
		//如待分析棋子棋型为五连
		m_LineRecord[nAnalyPos] = FIVE;
		return FIVE;
	}

	if (RightEdge - LeftEdge == 3)
	{
		//如待分析棋子棋型为四连
		bool Leftfour = false;
		if (LeftEdge>0)
			if (AnalyLine[LeftEdge - 1] == space)
				Leftfour = true;//左边有气

		if (RightEdge<GridNum)
			//右边未到边界
			if (AnalyLine[RightEdge + 1] == space)
				//右边有气
				if (Leftfour == true)//如左边有气
					m_LineRecord[nAnalyPos] = FOUR;//活四
				else
					m_LineRecord[nAnalyPos] = SFOUR;//冲四
			else
				if (Leftfour == true)//如左边有气
					m_LineRecord[nAnalyPos] = SFOUR;//冲四
				else
					if (Leftfour == true)//如左边有气
						m_LineRecord[nAnalyPos] = SFOUR;//冲四

		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 2)
	{
		//如待分析棋子棋型为三连
		bool LeftThree = false;

		if (LeftEdge>1)
			if (AnalyLine[LeftEdge - 1] == space)
				//左边有气
				if (LeftEdge>1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
				{
					//左边隔一空白有己方棋子
					m_LineRecord[LeftEdge] = SFOUR;//冲四
					m_LineRecord[LeftEdge - 2] = ANALSISED;
				}
				else
					LeftThree = true;

		if (RightEdge<GridNum)
			if (AnalyLine[RightEdge + 1] == space)
				//右边有气
				if (RightEdge<GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					//右边隔1个己方棋子
					m_LineRecord[RightEdge] = SFOUR;//冲四
					m_LineRecord[RightEdge + 2] = ANALSISED;
				}
				else
					if (LeftThree == true)//如左边有气
						m_LineRecord[RightEdge] = THREE;//活三
					else
						m_LineRecord[RightEdge] = STHREE; //冲三
			else
			{
				if (m_LineRecord[LeftEdge] == SFOUR)//如左冲四
					return m_LineRecord[LeftEdge];//返回

				if (LeftThree == true)//如左边有气
					m_LineRecord[nAnalyPos] = STHREE;//眠三
			}
		else
		{
			if (m_LineRecord[LeftEdge] == SFOUR)//如左冲四
				return m_LineRecord[LeftEdge];//返回
			if (LeftThree == true)//如左边有气
				m_LineRecord[nAnalyPos] = STHREE;//眠三
		}

		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 1)
	{
		//如待分析棋子棋型为二连
		bool Lefttwo = false;
		bool Leftthree = false;

		if (LeftEdge>2)
			if (AnalyLine[LeftEdge - 1] == space)
				//左边有气
				if (LeftEdge - 1>1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
					if (AnalyLine[LeftEdge - 3] == AnalyLine[LeftEdge])
					{
						//左边隔2个己方棋子
						m_LineRecord[LeftEdge - 3] = ANALSISED;
						m_LineRecord[LeftEdge - 2] = ANALSISED;
						m_LineRecord[LeftEdge] = SFOUR;//冲四
					}
					else
						if (AnalyLine[LeftEdge - 3] == space)
						{
							//左边隔1个己方棋子
							m_LineRecord[LeftEdge - 2] = ANALSISED;
							m_LineRecord[LeftEdge] = STHREE;//眠三
						}
						else
							Lefttwo = true;

		if (RightEdge<GridNum - 2)
			if (AnalyLine[RightEdge + 1] == space)
				//右边有气
				if (RightEdge + 1<GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
					if (AnalyLine[RightEdge + 3] == AnalyLine[RightEdge])
					{
						//右边隔两个己方棋子
						m_LineRecord[RightEdge + 3] = ANALSISED;
						m_LineRecord[RightEdge + 2] = ANALSISED;
						m_LineRecord[RightEdge] = SFOUR;//冲四
					}
					else
						if (AnalyLine[RightEdge + 3] == space)
						{
							//右边隔 1 个己方棋子
							m_LineRecord[RightEdge + 2] = ANALSISED;
							m_LineRecord[RightEdge] = STHREE;//眠三
						}
						else
						{
							if (m_LineRecord[LeftEdge] == SFOUR)//左边冲四
								return m_LineRecord[LeftEdge];//返回

							if (m_LineRecord[LeftEdge] == STHREE)//左边眠三        
								return m_LineRecord[LeftEdge];

							if (Lefttwo == true)
								m_LineRecord[nAnalyPos] = TWO;//返回活二
							else
								m_LineRecord[nAnalyPos] = STWO;//眠二
						}
				else
				{
					if (m_LineRecord[LeftEdge] == SFOUR)//冲四返回
						return m_LineRecord[LeftEdge];

					if (Lefttwo == true)//眠二
						m_LineRecord[nAnalyPos] = STWO;
				}

		return m_LineRecord[nAnalyPos];
	}

	return 0;
}

//将历史记录表中所有项目全置为初值
void AiAgent::ResetHistoryTable()
{
	memset(m_HistoryTable, 10, GRID_COUNT * sizeof(int));
}

//从历史得分表中取给定走法的历史得分
int AiAgent::GetHistoryScore(STONEMOVE* move)
{
	return m_HistoryTable[move->StonePos.x][move->StonePos.y];
}

//将一最佳走法汇入历史记录
void AiAgent::EnterHistoryScore(STONEMOVE* move, int depth)
{
	m_HistoryTable[move->StonePos.x][move->StonePos.y] += 2 << depth;
}

//对走法队列从小到大排序
//STONEMOVE* source原始队列
//STONEMOVE* target目标队列
//合并source[l…m]和 source[m +1…r]至target[l…r]
void AiAgent::Merge(STONEMOVE* source, STONEMOVE* target, int l, int m, int r)
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
	if (i>m)
		for (int q = j; q <= r; q++)
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)
			target[k++] = source[q];
}

void AiAgent::Merge_A(STONEMOVE* source, STONEMOVE* target, int l, int m, int r)
{
	//从大到小排序
	int i = l;
	int j = m + 1;
	int k = l;
	while (i <= m &&j <= r)
		if (source[i].Score >= source[j].Score)
			target[k++] = source[i++];
		else
			target[k++] = source[j++];
	if (i>m)
		for (int q = j; q <= r; q++)
			target[k++] = source[q];
	else
		for (int q = i; q <= m; q++)
			target[k++] = source[q];
}

//合并大小为 S 的相邻子数组
//direction 是标志,指明是从大到小还是从小到大排序
void AiAgent::MergePass(STONEMOVE* source, STONEMOVE* target, const int s, const int n, const bool direction)
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
	if (i + s<n)//剩余的元素个数小于2s
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
/*
 * 归并排序
 */
void AiAgent::MergeSort(STONEMOVE* source, int n, bool direction)
{
	int s = 1;
	while (s<n)
	{
		MergePass(source, m_TargetBuff, s, n, direction);
		s += s;
		MergePass(m_TargetBuff, source, s, n, direction);
		s += s;
	}
}
/*
 * 获取单层可行解
 */
int AiAgent::CreatePossibleMove(unsigned char position[][GRID_NUM], int nPly, int nSide)
{
	int i, j;
	m_nMoveCount = 0;
	for (i = 0; i<GRID_NUM; i++)
		for (j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] == (unsigned char)space)
				AddMove(j, i, nPly);
		}

	//使用历史启发类中的静态归并排序函数对走法队列进行排序
	//这是为了提高剪枝效率
	//        CHistoryHeuristic history;
	MergeSort(m_MoveList[nPly], m_nMoveCount, 0);

	return m_nMoveCount;//返回合法走法个数
}

//在m_MoveList中插入一个走法
//nToX是目标位置横坐标
//nToY是目标位置纵坐标
//nPly是此走法所在的层次
int AiAgent::AddMove(int nToX, int nToY, int nPly)
{
	m_MoveList[nPly][m_nMoveCount].StonePos.x = nToX;
	m_MoveList[nPly][m_nMoveCount].StonePos.y = nToY;

	m_nMoveCount++;
	m_MoveList[nPly][m_nMoveCount].Score = PosValue[nToY][nToX];//使用位置价值表评估当前走法的价值
	return m_nMoveCount;
}


/*
 * 使用α-β搜索一个最佳决策
 */
void AiAgent::SearchAGoodMove(unsigned char position[][GRID_NUM], int Type)
{
	int Score;

	memcpy(CurPosition, position, GRID_COUNT);
	m_nMaxDepth = m_nSearchDepth;
	m_pTranspositionTable->CalculateInitHashKey(CurPosition);
	ResetHistoryTable();
	Score = NegaScout(m_nMaxDepth, -20000, 20000);
	X = m_cmBestMove.StonePos.y;
	Y = m_cmBestMove.StonePos.x;
	MakeMove(&m_cmBestMove, Type);
	memcpy(position, CurPosition, GRID_COUNT);
}
/*
 * 判断游戏是否结束
 */
int AiAgent::IsGameOver(unsigned char position[][GRID_NUM], int nDepth)
{
	int score, i;//计算要下的棋子颜色
	i = (m_nMaxDepth - nDepth) % 2;
	score = Eveluate(position, i);//调用估值函数
	if (abs(score)>8000)//如果估值函数返回极值，给定局面游戏结束
		return score;//返回极值
	return 0;//返回未结束
}
/*
 * 副极大值风格的α-β搜索
 */
int AiAgent::NegaScout(int depth, int alpha, int beta)
{
	int Count, i;
	unsigned char type;
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
	side = (m_nMaxDepth - depth) % 2;//计算当前节点的类型,极大0/极小1
	score = m_pTranspositionTable->LookUpHashTable(alpha, beta, depth, side);
	if (score != 66666)
		return score;
	if (depth <= 0)//叶子节点取估值
	{
		score = Eveluate(CurPosition, side);
		m_pTranspositionTable->EnterHashTable(exact, score, depth, side);//将估值存入置换表

		return score;
	}
	Count = CreatePossibleMove(CurPosition, depth, side);
	for (i = 0; i<Count; i++)
		m_MoveList[depth][i].Score = GetHistoryScore(&m_MoveList[depth][i]);

	MergeSort(m_MoveList[depth], Count, 0);
	int bestmove = -1;
	a = alpha;
	b = beta;

	int eval_is_exact = 0;

	for (i = 0; i<Count; i++)
	{
		type = MakeMove(&m_MoveList[depth][i], side);
		m_pTranspositionTable->Hash_MakeMove(&m_MoveList[depth][i], CurPosition);
		t = -NegaScout(depth - 1, -b, -a);//递归搜索子节点，对第 1 个节点是全窗口，其后是空窗探测
		if (t>a && t<beta && i>0)
		{
			//对于第一个后的节点,如果上面的搜索failhigh
			a = -NegaScout(depth - 1, -beta, -t);//re-search
			eval_is_exact = 1;//设数据类型为精确值
			if (depth == m_nMaxDepth)
				m_cmBestMove = m_MoveList[depth][i];
			bestmove = i;
		}
		m_pTranspositionTable->Hash_UnMakeMove(&m_MoveList[depth][i], CurPosition);
		UnMakeMove(&m_MoveList[depth][i]);
		if (a<t)
		{
			eval_is_exact = 1;
			a = t;
			if (depth == m_nMaxDepth)
				m_cmBestMove = m_MoveList[depth][i];
		}
		if (a >= beta)
		{
			m_pTranspositionTable->EnterHashTable(lower_bound, a, depth, side);
			EnterHistoryScore(&m_MoveList[depth][i], depth);
			return a;
		}
		b = a + 1;                      /* set new null window */
	}
	if (bestmove != -1)
		EnterHistoryScore(&m_MoveList[depth][bestmove], depth);
	if (eval_is_exact)
		m_pTranspositionTable->EnterHashTable(exact, a, depth, side);
	else
		m_pTranspositionTable->EnterHashTable(upper_bound, a, depth, side);

	return a;
}
/*
 * 执行一个决策
 */
unsigned char AiAgent::MakeMove(STONEMOVE* move, int type)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = type;
	return 0;
}
/*
 * 撤销一个决策
 */
void AiAgent::UnMakeMove(STONEMOVE* move)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = space;
}
/*
* 构造函数
*/
AiAgent::AiAgent()
{	
	m_pTranspositionTable = new TranspositionTable();
	colour = black;
	m_nUserStoneColor = white;
	for (int i = 0; i < GRID_NUM; i++)
		for (int j = 0; j < GRID_NUM; j++)
			m_RenjuBoard[i][j] = space; //棋盘初始化 

}
/*
* 传入人类的行动,获取AiAgent的行动
*/
void AiAgent::GetAiAction(POINT*PlayerPos)
{
	qDebug() << "GetAiAction is running...";
	int XS;
	POINT *temp=new POINT;
	if (XS == 1 && PlayerPos->x == -1 && PlayerPos->y == -1) //如果己方执黑且是第一步，则占据棋盘中心位置 
	{
		m_RenjuBoard[GRID_NUM / 2][GRID_NUM / 2] = colour; //更新棋盘信息
	}
	else
	{
		m_RenjuBoard[PlayerPos->x][PlayerPos->y] = m_nUserStoneColor;
		m_nSearchDepth = 4;//AI难度等级设置 
						   //最大搜索深度        
		ResetHistoryTable();
		m_pTranspositionTable->InitializeHashKey();//建立哈希表，创建随机数组
		SearchAGoodMove(m_RenjuBoard, colour);
		m_RenjuBoard[X][Y] = colour;
		//输出 
		
		temp->x = X;
		temp->y = Y;
		std::cout << std::flush; //刷新缓冲区
		m_pTranspositionTable->_CTranspositionTable();
	}
	emit AIComplete(temp);//带AI的落子点发出
}