#include "PositionStruct.h"
#include <cstring>
#include <qDebug>

void PositionStruct::Startup(void)
{
	sdPlayer = 0;
	
	//memcpy(ucpcSquares, cucpcStartup, GRID_COUNT);
	for(int i=0;i<GRID_COUNT;i++)
	{
		ucpcSquares[i] = -1;
	}
}
/*
 * 交换走子方
 */
void PositionStruct::ChangeSide()
{
	sdPlayer = 1 - sdPlayer;
}
/*
 * 在棋盘上放一枚棋子
 * color是棋子类型(0黑1白),pos是位置
 */
void PositionStruct::AddPiece(int pos, int color)
{
	// 在棋盘上放一枚棋子
	ucpcSquares[pos] = color;
	if (color == 1)
	{
		vlWhite += PosValue[pos];
	}
	else
	{
		vlBlack += PosValue[pos];
	}
}
/*
 *从棋盘上拿走一枚棋子
 *color是棋子类型(0黑1白),pos是位置
 */
void PositionStruct::DelPiece(int pos, int color)
{
	ucpcSquares[pos] = -1;
	if (color == 1)
	{
		vlWhite -= PosValue[pos];
	}
	else
	{
		vlBlack -= PosValue[pos];
	}
}
/*
 * 局面评价函数(待修改)
 */
int PositionStruct::Evaluate()
{
	int position[GRID_NUM][GRID_NUM];
	//将棋盘转为二维数组
	for (int i=0;i<GRID_NUM;i++)
	{
		for (int j=0;j<GRID_NUM;j++)
		{
			position[i][j] = ucpcSquares[j*15+i];
		}
	}
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
					Analysis_horizon(position, i, j);

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
	Color _ourSide = sdPlayer==0?black:white;//我方颜色
	Color _counterSide = (_ourSide == white ? black : white);//对方颜色
	//Color _ourSide = white;
	//Color _counterSide = black;
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
		return -99990;
	}
	//活四,双冲四,冲四活三
	if (TypeCount[_ourSide][Alive4] >= 1 || TypeCount[_ourSide][Sleep4] >= 2 || (TypeCount[_ourSide][Sleep4] >= 1 && TypeCount[_ourSide][Alive3 >= 1]))
	{
		return 10000;
	}
	if (TypeCount[_counterSide][Alive4] >= 1 || TypeCount[_counterSide][Sleep4] >= 2 || (TypeCount[_counterSide][Sleep4] >= 1 && TypeCount[_counterSide][Alive3 >= 1]))
	{
		return -9990;
	}
	//=====开始打分,注意不打负分====
	//双活三
	if (TypeCount[_ourSide][Alive3] >= 2)
	{
		_ourScore += 5000;
	}
	if (TypeCount[_counterSide][Alive3] >= 2)
	{
		_counterScore += 4990;
	}
	//活三眠三
	if (TypeCount[_ourSide][Alive3] >= 1 && TypeCount[_ourSide][Sleep3] >= 1)
	{
		_ourScore += 1000;
	}
	if (TypeCount[_counterSide][Alive3] >= 1 && TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += 990;
	}
	//眠四
	if (TypeCount[_ourSide][Sleep4] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep4] * 500;
	}
	if (TypeCount[_counterSide][Sleep4] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep4] * 490;
	}
	//活三
	if (TypeCount[_ourSide][Alive3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive3] * 200;
	}
	if (TypeCount[_counterSide][Alive3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive3] * 190;
	}
	//双活二
	if (TypeCount[_ourSide][Alive2] >= 2)
	{
		_ourScore += 100;
	}
	if (TypeCount[_counterSide][Alive2] >= 2)
	{
		_counterScore += 90;
	}
	//眠三
	if (TypeCount[_ourSide][Sleep3] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep3] * 50;
	}
	if (TypeCount[_counterSide][Sleep3] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep3] * 40;
	}
	//活二眠二
	if (TypeCount[_ourSide][Alive2] >= 1 && TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += 10;
	}
	if (TypeCount[_counterSide][Alive2] >= 1 && TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += 10;
	}
	//活二
	if (TypeCount[_ourSide][Alive2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Alive2] * 5;
	}
	if (TypeCount[_counterSide][Alive2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Alive2] * 5;
	}
	//眠二
	if (TypeCount[_ourSide][Sleep2] >= 1)
	{
		_ourScore += TypeCount[_ourSide][Sleep2] * 3;
	}
	if (TypeCount[_counterSide][Sleep2] >= 1)
	{
		_counterScore += TypeCount[_counterSide][Sleep2] * 3;
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
					_ourScore += PosValue[i*GRID_NUM+j];
				else
					_counterScore += PosValue[i*GRID_NUM + j];
			}

		}
	}
	//qDebug() << "_ourScore - _counterScore" << _ourScore - _counterScore;
	return _ourScore - _counterScore;

	//=============================================================================
	//return (sdPlayer == 0 ? vlWhite - vlBlack : vlBlack - vlWhite) + ADVANCED_VALUE;
}
/*
 * 走一步棋
 */
void PositionStruct::MakeMove(int pos)
{
	AddPiece(pos, sdPlayer);
	nDistance++;
	ChangeSide();
}
/*
 * 撤消走一步棋
 */
void PositionStruct::UndoMakeMove(int pos)
{ 
	DelPiece(pos, sdPlayer);
	nDistance--;
	ChangeSide();
}
/*
 * 生成所有走法,将所有可行的落子点放在mvs里面,返回可行解个数
 */
int PositionStruct::GenerateMoves(int* mvs) 
{
	/*
	 * 只搜索有棋子范围的3格子邻域
	 */
	int j = 0;
	for (int i=0;i<GRID_COUNT;i++)
	{
		if (ucpcSquares[i] == -1)//这是一个空格
		{
			if(NeighborSum(i)>-25)//以这个格子为中心的5×5范围内有任何一方的棋子
			{
				mvs[j] = i;
				j++;				
			}

		}
	}
	return j;
}
/*
 * 判断走法是否合理
 */
bool PositionStruct::LegalMove(int pos) const
{
	if (ucpcSquares[pos]!=-1)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}
/*
 *求某棋子的5*5邻域和
 */
int PositionStruct::NeighborSum(int pos)
{
	int sum = 0;
	int y = pos / GRID_NUM;
	int x = pos - y*GRID_NUM;

	int X[5];
	int Y[5];
	int n = 0;
	for (int i=-2;i<3;i++)
	{
		X[n] = x + i;
		Y[n] = y + i;
		n++;
	}
	for (int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			if(X[i]>=0&&Y[j]>=0 && X[i] <= 14 && Y[j] <= 14)
			{
				sum += ucpcSquares[Y[j] * 15 + X[i]];
			}else
			{
				sum += -1;
			}
		}
	}
	return sum;
}

int PositionStruct::AnalysisLine(int * position, int GridNum, int StonePos)
{
	int StoneType;
	int AnalyLine[30];
	int nAnalyPos;
	int LeftEdge, RightEdge;
	int LeftRange, RightRange;

	if (GridNum<5)
	{
		//数组长度小于5没有意义
		memset(m_LineRecord, ANALSISED, sizeof(int)*GridNum);
		return 0;
	}
	nAnalyPos = StonePos;
	memset(m_LineRecord, TOBEANALSIS, sizeof(int) * 30);
	memset(AnalyLine, 0, sizeof(int) * 30);
	//将传入数组装入AnalyLine;
	memcpy(&AnalyLine, position, GridNum*sizeof(int));
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

int PositionStruct::AnalysisMainDiagonal(int position[][GRID_NUM], int i, int j)
{
	int tempArray[GRID_NUM];
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

int PositionStruct::AnalysisVertical(int position[][GRID_NUM], int i, int j)
{
	int tempArray[GRID_NUM];
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

int PositionStruct::Analysis_horizon(int position[][15], int i, int j)
{
	//调用直线分析函数分析
	AnalysisLine(position[i], 15, j);
	//拾取分析结果
	for (int s = 0; s<15; s++)
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[i][s][0] = m_LineRecord[s];

	return TypeRecord[i][j][0];
}

int PositionStruct::AnalysisSubDiagonal(int position[][15], int i, int j)
{
	int tempArray[GRID_NUM];
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
