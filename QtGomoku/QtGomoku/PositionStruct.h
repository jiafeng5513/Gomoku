#pragma once
#include "StaticDefinition.h"

struct PositionStruct {
	int sdPlayer;                               // 轮到谁走，0=黑棋，1=白棋(0先行)
	int ucpcSquares[GRID_COUNT];                // 棋盘
	int vlWhite, vlBlack;                       // 双方的总价值
	int nDistance;                              // 当前情况是从根节点出发下几步到达的
	int TypeRecord[GRID_NUM][GRID_NUM][4];		// TypeRecord[x][y][i]表示棋子xy在i方向上的棋形分析结果
												// i:0=水平,1==垂直,2==主方向,3==次方向
	int TypeCount[2][20];						// 存放统记过的分析结果的数组,TypeCount[i][j]元素表示第i中颜色的棋子的第j种棋形的数量
	int m_LineRecord[30];						// 存放AnalysisLine分析结果的数组

	void Startup(void);                         // 初始化棋盘
	void ChangeSide(void);			            // 交换走子方
	void AddPiece(int pos, int color);          // color是棋子类型(0黑1白),pos是位置
	void DelPiece(int pos, int color);          // 从棋盘上拿走一枚棋子
	int Evaluate(void);							// 估值函数
	void MakeMove(int mv);						// 走一步棋
	void UndoMakeMove(int mv);					// 撤销走一步棋
	int GenerateMoves(int *mvs) ;				// 生成所有走法
	bool LegalMove(int mv) const;               // 判断走法是否合理

	int NeighborSum(int pos);					//求某棋子的邻域和
	int AnalysisLine(int* position, int GridNum, int StonePos);
	int AnalysisSubDiagonal(int position[][GRID_NUM], int i, int j);
	int AnalysisMainDiagonal(int position[][GRID_NUM], int i, int j);
	int AnalysisVertical(int position[][GRID_NUM], int i, int j);
	int Analysis_horizon(int position[][GRID_NUM], int i, int j);
};