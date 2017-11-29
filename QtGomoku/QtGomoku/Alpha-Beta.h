/*
 * 使用改进版的alpha-beta剪枝算法构造五子棋AiAgent
 */
#pragma once
#include "StaticDefinition.h"
#include "TranspositionTable.h"
class AiAgent
{
public:
	AiAgent();
	POINT GetAiAction(int PlayerX, int PlayerY);
private:
	TranspositionTable *m_pTranspositionTable;
	int m_HistoryTable[GRID_NUM][GRID_NUM];//历史得分表

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
	int TypeRecord[GRID_NUM][GRID_NUM][4];//存放全部分析结果的数组,有三个维度,用于存放水平、垂直、左斜、右斜 4 个方向上所有棋型分析结果
	int TypeCount[2][20];          //存放统记过的分析结果的数组
	//位置重要性价值表,此表从中间向外,越往外价值越低
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
	int AnalysisRight(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisLeft(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisVertical(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisHorizon(unsigned char position[][GRID_NUM], int i, int j);
	int Eveluate(unsigned char position[][GRID_NUM], bool bIsWhiteTurn);
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
};