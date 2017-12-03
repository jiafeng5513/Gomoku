#include <cstdlib>
#include <iostream>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"StaticDefinition.h"
#include"Alpha-Beta.h"

#include <qDebug>
#include <valarray>

/*
* 估值函数
* 参数1:要评价的局面(存储整个局面的一个数组)
* 参数2:我方的颜色
* 1.估值函数在决策时的叶节点被调用
* 2.估值函数评价的是叶节点这个局势的好坏
* 3.评价原理:
*			如果发现必胜局面,提前停止判断,直接返回一个较大的估值
*			计算当前局面我方的棋形评分
*			计算当前局面对方的棋形评分
*			返回:我方评分-对方评分(策略是选择对我方最有利,对对方最不利的路径)
*/
int AiAgent::Eveluate(unsigned char position[][GRID_NUM], Color color)
{
	count++;//计数器累加
	/*
	* void *memset(void *s, int ch, size_t n)
	* 函数解释：将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 。
	* memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法
	*/
	//清空棋型分析结果
	memset(TypeRecord, TOBEANALSIS, GRID_COUNT * 4 * 4);
	memset(TypeCount, 0, 40 * 4);

	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] != space)//当前分析的位置有棋子
			{
				//如果水平方向上没有分析过
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);

				//如果垂直方向上没有分析过
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);

				//如果主对角线方向上没有分析过
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisMainDiagonal(position, i, j);

				//如果次对角线方向上没有分析过
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisSubDiagonal(position, i, j);
			}
		}
	}
		

	//对分析结果进行统计,得到每种棋型的数量
	//这种方基于一个假设:对于单一的落子,在同一方向上不可能同时形成两种或两种以上棋形
	//如果上述假设不成立,整个估值函数失效
	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			for (int k = 0; k<4; k++)
			{
				if (position[i][j] != space)
				{
					switch (TypeRecord[i][j][k])
					{
					case Five://五连
						TypeCount[position[i][j]][Five]++;
						break;
					case Alive4://活四
						TypeCount[position[i][j]][Alive4]++;
						break;
					case Sleep4://冲四
						TypeCount[position[i][j]][Sleep4]++;
						break;
					case Alive3://活三
						TypeCount[position[i][j]][Alive3]++;
						break;
					case Sleep3://眠三
						TypeCount[position[i][j]][Sleep3]++;
						break;
					case Alive2://活二
						TypeCount[position[i][j]][Alive2]++;
						break;
					case Sleep2://眠二
						TypeCount[position[i][j]][Sleep2]++;
						break;
					default:
						break;
					}
				}
			}
		}
	}
	//========开始评估========
	int _ourScore = 0, _counterScore = 0;//我方评分,对方评分
	Color _ourSide = color;//我方颜色
	Color _counterSide = (color == white ? black : white);//对方颜色
	/*
	 *必胜/必败只有两种情况:
	 * 1.连五
	 * 2.活四,双冲四,冲四活三
	 * 同时遇到两种情况的时候,如果是我方,优先选择形成连五
	 * 如果是对方,我们认为情况二对我们还有一线生机
	 * 
	 * 其他的情况计算局势分
	 */
	//连五
	if (TypeCount[_ourSide][Five] >= 1)
	{
		return 100000;
	}
	if (TypeCount[_counterSide][Five] >= 1)
	{
		return -100000;
	}
	//活四,双冲四,冲四活三
	if (TypeCount[_ourSide][Alive4]>=1||TypeCount[_ourSide][Sleep4]>=2||(TypeCount[_ourSide][Sleep4]>=1&& TypeCount[_ourSide][Alive3>=1]))
	{
		return 10000;
	}
	if (TypeCount[_counterSide][Alive4]>= 1||TypeCount[_counterSide][Sleep4]>= 2||(TypeCount[_counterSide][Sleep4]>=1&&TypeCount[_counterSide][Alive3 >= 1]))
	{
		return -10000;
	}
	//=====开始打分,注意不打负分====
	//双活三
	if (TypeCount[_ourSide][Alive3] >= 2)
	{
		_ourScore += 5000;
	}
	if (TypeCount[_counterSide][Alive3] >= 2)
	{
		_counterScore += 5000;
	}
	//活三眠三
	if (TypeCount[_ourSide][Alive3] >= 1&& TypeCount[_ourSide][Sleep3]>=1)
	{
		_ourScore += 1000;
	}
	if (TypeCount[_counterSide][Alive3] >= 1 && TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += 1000;
	}
	//眠四
	if (TypeCount[_ourSide][Sleep4] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep4]*500;
	}
	if (TypeCount[_counterSide][Sleep4] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep4]*500;
	}
	//活三
	if (TypeCount[_ourSide][Alive3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive3]*200;
	}
	if (TypeCount[_counterSide][Alive3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive3]*200;
	}
	//双活二
	if (TypeCount[_ourSide][Alive2] >= 2)
	{
		_ourScore += 100;
	}
	if (TypeCount[_counterSide][Alive2] >= 2)
	{
		_counterScore += 100;
	}
	//眠三
	if (TypeCount[_ourSide][Sleep3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep3]*50;
	}
	if (TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep3]*50;
	}
	//活二眠二
	if (TypeCount[_ourSide][Alive2] >= 1&& TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += 10;
	}
	if (TypeCount[_counterSide][Alive2] >= 1&& TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += 10;
	}
	//活二
	if (TypeCount[_ourSide][Alive2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive2]*5;
	}
	if (TypeCount[_counterSide][Alive2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive2]*5;
	}
	//眠二
	if (TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep2]*3;
	}
	if (TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep2]*3;
	}
	/*
	 * 死四,死三,死二,四一
	 */
	//======打中央权重分=====
	//加上所有棋子的位置价值
	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			if (position[i][j] != space)
			{
				if (position[i][j] == _ourSide)
					_ourScore += PosValue[i][j];
				else
					_counterScore += PosValue[i][j];
			}
				
		}
	}
		
//================打分结束=========================
	return _ourScore - _counterScore;
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
 *分析棋盘上某点主对角线方向上的棋型
 */
int AiAgent::AnalysisMainDiagonal(unsigned char position[][GRID_NUM], int i, int j)
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
 *分析棋盘上某点在次对角线方向上的棋型
 */
int AiAgent::AnalysisSubDiagonal(unsigned char position[][GRID_NUM], int i, int j)
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
		m_LineRecord[nAnalyPos] = Five;
		return Five;
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
					m_LineRecord[nAnalyPos] = Alive4;//活四
				else
					m_LineRecord[nAnalyPos] = Sleep4;//冲四
			else
				if (Leftfour == true)//如左边有气
					m_LineRecord[nAnalyPos] = Sleep4;//冲四
				else
					if (Leftfour == true)//如左边有气
						m_LineRecord[nAnalyPos] = Sleep4;//冲四

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
					m_LineRecord[LeftEdge] = Sleep4;//冲四
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
					m_LineRecord[RightEdge] = Sleep4;//冲四
					m_LineRecord[RightEdge + 2] = ANALSISED;
				}
				else
					if (LeftThree == true)//如左边有气
						m_LineRecord[RightEdge] = Alive3;//活三
					else
						m_LineRecord[RightEdge] = Sleep3; //冲三
			else
			{
				if (m_LineRecord[LeftEdge] == Sleep4)//如左冲四
					return m_LineRecord[LeftEdge];//返回

				if (LeftThree == true)//如左边有气
					m_LineRecord[nAnalyPos] = Sleep3;//眠三
			}
		else
		{
			if (m_LineRecord[LeftEdge] == Sleep4)//如左冲四
				return m_LineRecord[LeftEdge];//返回
			if (LeftThree == true)//如左边有气
				m_LineRecord[nAnalyPos] = Sleep3;//眠三
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
						m_LineRecord[LeftEdge] = Sleep4;//冲四
					}
					else
						if (AnalyLine[LeftEdge - 3] == space)
						{
							//左边隔1个己方棋子
							m_LineRecord[LeftEdge - 2] = ANALSISED;
							m_LineRecord[LeftEdge] = Sleep3;//眠三
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
						m_LineRecord[RightEdge] = Sleep4;//冲四
					}
					else
						if (AnalyLine[RightEdge + 3] == space)
						{
							//右边隔 1 个己方棋子
							m_LineRecord[RightEdge + 2] = ANALSISED;
							m_LineRecord[RightEdge] = Sleep3;//眠三
						}
						else
						{
							if (m_LineRecord[LeftEdge] == Sleep4)//左边冲四
								return m_LineRecord[LeftEdge];//返回

							if (m_LineRecord[LeftEdge] == Sleep3)//左边眠三        
								return m_LineRecord[LeftEdge];

							if (Lefttwo == true)
								m_LineRecord[nAnalyPos] = Alive2;//返回活二
							else
								m_LineRecord[nAnalyPos] = Sleep2;//眠二
						}
				else
				{
					if (m_LineRecord[LeftEdge] == Sleep4)//冲四返回
						return m_LineRecord[LeftEdge];

					if (Lefttwo == true)//眠二
						m_LineRecord[nAnalyPos] = Sleep2;
				}

		return m_LineRecord[nAnalyPos];
	}

	return 0;
}

//将历史记录表中所有项目全置为初值
void AiAgent::ResetHistoryTable()
{
	memset(m_HistoryTable, 0, GRID_COUNT * sizeof(int));
}

//从历史得分表中取给定走法的历史得分
int AiAgent::GetHistoryScore(STONEMOVE* move)
{
	return m_HistoryTable[move->StonePos.x][move->StonePos.y];
}

/*
 * 将一最佳走法汇入历史记录,走法在历史表中的值是深度的平方
 */
void AiAgent::EnterHistoryScore(STONEMOVE* move, int depth)
{
	m_HistoryTable[move->StonePos.x][move->StonePos.y] += 2 << depth;//右移两位,乘4
	m_HistoryTable[move->StonePos.x][move->StonePos.y] += std::pow(depth, 2);
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
 * 目前是全局遍历,更好的方法是对有棋子的地方进行邻域搜索
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
	Score = NegaScout(m_nMaxDepth, -100000, 100000);
	//Score=SearchFull(-MATE_VALUE, MATE_VALUE, Max_Level);
	X = m_cmBestMove.StonePos.y;
	Y = m_cmBestMove.StonePos.x;
	qDebug() << "Ai's Pos: (" << X << "," << Y << ")" << "  Score:" << m_cmBestMove.Score;
	MakeMove(&m_cmBestMove, Type);
	memcpy(position, CurPosition, GRID_COUNT);
}
/*
 * 判断游戏是否结束
 */
int AiAgent::IsGameOver(unsigned char position[][GRID_NUM], int nDepth)
{
	int score = Eveluate(position, ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));//调用估值函数
	if (abs(score)>8000)//如果估值函数返回极值，给定局面游戏结束
		return score;//返回极值
	return 0;//返回未结束
}
/*
 * 副极大值风格的α-β搜索
 * 参数:depth:向下收搜索的深度
 *		alpha:α,下界
 *		beat :β,上界
 */
int AiAgent::NegaScout(int depth, int alpha, int beta)
{
	int vlBest = -MATE_VALUE;
	/*        if(depth>0)
	{
	i= IsGameOver(CurPosition,depth);
	if(i!=0)
	return i;  //已分胜负，返回极值
	}
	*/
	int side = (m_nMaxDepth - depth) % 2;//计算当前节点的类型,极大0/极小1
	int score = m_pTranspositionTable->LookUpHashTable(alpha, beta, depth, side);
	if (score != 66666)
		return score;
	//====1=====递归出口:返回局面估值
	if (depth <= 0)//到达最底层(目前是由于最大深度耗尽)
	{
		score = Eveluate(CurPosition, ((m_nMaxDepth - depth) % 2 == 0 ? black : white));
		//qDebug() << "Eveluate=" << score;
		m_pTranspositionTable->EnterHashTable(exact, score, depth, side);//将估值存入置换表
		return score;
	}
	//====2=====生成可行解,并按照历史表排序
	int numOfPossibleMove = CreatePossibleMove(CurPosition, depth, side);//本层可行解数量
	for (int i = 0; i<numOfPossibleMove; i++)
	{
		m_MoveList[depth][i].Score = GetHistoryScore(&m_MoveList[depth][i]);
		//qDebug() << "HistoryScore=" << m_MoveList[depth][i].Score;
	}
	MergeSort(m_MoveList[depth], numOfPossibleMove, 1);
	int bestmove = -1;
	int a = alpha;
	int b = beta;

	int eval_is_exact = 0;
	//====3=====逐步尝试可行解,并进行递归
	for (int i = 0; i<numOfPossibleMove; i++)
	{
		unsigned char type = MakeMove(&m_MoveList[depth][i], side);
		m_pTranspositionTable->Hash_MakeMove(&m_MoveList[depth][i], CurPosition);
		int t = -NegaScout(depth - 1, -b, -a);//递归搜索子节点，对第 1 个节点是全窗口，其后是空窗探测
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



// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
int AiAgent::SearchFull(int vlAlpha, int vlBeta, int nDepth) {
	int i, nGenMoves, pcCaptured;
	int vl, vlBest;
	STONEMOVE*mvBest = new STONEMOVE;
	//int mvs[GRID_COUNT];
	// 一个Alpha-Beta完全搜索分为以下几个阶段

	// 1. 到达水平线，则返回局面评价值
	if (nDepth == 0) {
		return  Eveluate(CurPosition, ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));
	}

	// 2. 初始化最佳值和最佳走法
	vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)
	//mvBest = 0;           // 这样可以知道，是否搜索到了Beta走法或PV走法，以便保存到历史表

	// 3. 生成全部走法，并根据历史表排序
	nGenMoves = CreatePossibleMove(CurPosition, nDepth, ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));//本层可行解数量
	//nGenMoves = pos.GenerateMoves(mvs);
	//按照历史表对CurPosition排序=======================此处需要注意
	for (int i = 0; i<nGenMoves; i++)
	{
		m_MoveList[nDepth][i].Score = GetHistoryScore(&m_MoveList[nDepth][i]);
		//qDebug() << "HistoryScore=" << m_MoveList[depth][i].Score;
	}
	MergeSort(m_MoveList[nDepth], nGenMoves, 0);
	//qsort(m_MoveList[nDepth], nGenMoves, sizeof(STONEMOVE), CompareHistory);
	SortByHistory(m_MoveList[nDepth], nGenMoves);
	// 4. 逐一走这些走法，并进行递归
	for (i = 0; i < nGenMoves; i++) {
		unsigned char type = MakeMove(&m_MoveList[nDepth][i], ((m_nMaxDepth - nDepth) % 2 == 0 ? black : white));
		vl = -SearchFull(-vlBeta, -vlAlpha, nDepth - 1);
		UnMakeMove(&m_MoveList[nDepth][i]);

		//pos.UndoMakeMove(mvs[i], pcCaptured);

		// 5. 进行Alpha-Beta大小判断和截断
		if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
			vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
			if (vl >= vlBeta) { // 找到一个Beta走法
				*mvBest = m_MoveList[nDepth][i];  // Beta走法要保存到历史表
				break;            // Beta截断
			}
			if (vl > vlAlpha) { // 找到一个PV走法
				*mvBest = m_MoveList[nDepth][i];  // PV走法要保存到历史表
				vlAlpha = vl;     // 缩小Alpha-Beta边界
			}
		}

	}

	// 5. 所有走法都搜索完了，把最佳走法(不能是Alpha走法)保存到历史表，返回最佳值
	if (vlBest == -MATE_VALUE) {
		// 如果是杀棋，就根据杀棋步数给出评价
		return (Max_Level-nDepth) - MATE_VALUE;
	}
	if (mvBest->StonePos.x == -1&& mvBest->StonePos.y == -1) {
		// 如果不是Alpha走法，就将最佳走法保存到历史表
		m_HistoryTable[(int)(mvBest->StonePos.x)][(int)(mvBest->StonePos.y)]+= nDepth * nDepth;
		if (nDepth == Max_Level) {
			// 搜索根节点时，总是有一个最佳走法(因为全窗口搜索不会超出边界)，将这个走法保存下来
			//Search.mvResult = mvBest;
			m_cmBestMove = *mvBest;
		}
	}
	return vlBest;
}
/*
 * 按照历史表排序
 */
void AiAgent::SortByHistory(STONEMOVE*list,int length)
{	 
	bool swaped;
	int n = length;
	do {
		swaped = false;
		for (int i = 1; i < n; i++) {
			if (m_HistoryTable[(int)(list[i].StonePos.x)][(int)(list[i].StonePos.y)]<m_HistoryTable[(int)(list[i].StonePos.x)][(int)(list[i].StonePos.y)]) {
				STONEMOVE temp = list[i - 1];
				list[i - 1] = list[i];
				list[i] = temp;
				swaped = true;
			}
		}
		n--;
	} while (swaped);
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