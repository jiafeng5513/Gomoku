/*
 * 使用改进版的alpha-beta剪枝算法构造五子棋AiAgent
 */
#pragma once
#include "StaticDefinition.h"
#include "TranspositionTable.h"
#include <qobject.h>

class AiAgent:public QObject
{
	Q_OBJECT

public:
	AiAgent();
	 int m_HistoryTable[GRID_NUM][GRID_NUM];//历史得分表
private:
	TranspositionTable *m_pTranspositionTable;
	

	STONEMOVE m_TargetBuff[225];    //排序用的缓冲队列
	STONEMOVE m_MoveList[10][225];//用以记录走法的数组
	STONEMOVE m_cmBestMove;        //记录最佳走法的变量        
								   //CMoveGenerator* m_pMG;        //走法产生器指针        
								   //CEveluation* m_pEval;        //估值核心指针   
	unsigned char m_LineRecord[30];          //存放AnalysisLine分析结果的数组
	unsigned char CurPosition[GRID_NUM][GRID_NUM];//搜索时用于当前节点棋盘状态的数组
	unsigned char m_RenjuBoard[GRID_NUM][GRID_NUM];//棋盘数组，用于显示棋盘


	int m_nMoveCount;//此变量用以记录走法的总数
	int m_nSearchDepth;        //最大搜索深度
	int m_nMaxDepth;        //当前搜索的最大搜索深度
	int m_nUserStoneColor;         //用户棋子的颜色
	int X, Y;                               //AI输出落子位置 			 
	int count = 0;//全局变量,用以统计估值函数的执行遍数
	int colour;
	int TypeRecord[GRID_NUM][GRID_NUM][4];//TypeRecord[x][y][i]表示棋子xy在i方向上的棋形分析结果
										  //i:0=水平,1==垂直,2==主方向,3==次方向
	int TypeCount[2][20];          //存放统记过的分析结果的数组,TypeCount[i][j]元素表示第i中颜色的棋子的第j种棋形的数量
	int PosValue[GRID_NUM][GRID_NUM] =
	{
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
		{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
		{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
		{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
		{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,7,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,6,6,6,5,4,3,2,1,0 },
		{ 0,1,2,3,4,5,5,5,5,5,4,3,2,1,0 },
		{ 0,1,2,3,4,4,4,4,4,4,4,3,2,1,0 },
		{ 0,1,2,3,3,3,3,3,3,3,3,3,2,1,0 },
		{ 0,1,2,2,2,2,2,2,2,2,2,2,2,1,0 },
		{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
	};

private:
	int AnalysisLine(unsigned char* position, int GridNum, int StonePos);
	int AnalysisSubDiagonal(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisMainDiagonal(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisVertical(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisHorizon(unsigned char position[][GRID_NUM], int i, int j);
	int Eveluate(unsigned char position[][GRID_NUM], Color color);//估值函数
	int AddMove(int nToX, int nToY, int nPly);
	int CreatePossibleMove(unsigned char position[][GRID_NUM], int nPly, int nSide);
	void MergeSort(STONEMOVE* source, int n, bool direction);
	void MergePass(STONEMOVE* source, STONEMOVE* target, const int s, const int n, const bool direction);
	void Merge_A(STONEMOVE* source, STONEMOVE* target, int l, int m, int r);
	void Merge(STONEMOVE* source, STONEMOVE* target, int l, int m, int r);
	void EnterHistoryScore(STONEMOVE* move, int depth);
	int GetHistoryScore(STONEMOVE* move);
	void ResetHistoryTable();
	int NegaScout(int depth, int alpha, int beta);
	void SearchAGoodMove(unsigned char position[][GRID_NUM], int Type);
	int IsGameOver(unsigned char position[][GRID_NUM], int nDepth);
	void UnMakeMove(STONEMOVE* move);
	unsigned char MakeMove(STONEMOVE* move, int type);
	int SearchFull(int vlAlpha, int vlBeta, int nDepth);
	void SortByHistory(STONEMOVE*,int);
public slots:
	void GetAiAction(POINT*);
	signals:
	void AIComplete(POINT*);//带AI的落子点返回,计算完毕
};
/*
 *现在AI的问题:
 *1.强度不够
 *2.一旦判断进入必败局面,就会走0.0点
 *
 */