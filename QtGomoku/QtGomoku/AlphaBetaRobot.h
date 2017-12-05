#pragma once
#include "PositionStruct.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <qobject.h>
/*
 * 局面实例
 */
static PositionStruct situation;
/*
 * 与搜索有关的全局变量
 */	 
static struct {
	int mvResult;             // 电脑走的棋
	int nHistoryTable[GRID_COUNT]; // 历史表
} Search;
/*
 * qsort快速排序按历史表排序的比较函数
 */
static int CompareHistory(const void *lpmv1, const void *lpmv2) {
	return Search.nHistoryTable[*(int *)lpmv2] - Search.nHistoryTable[*(int *)lpmv1];
}



class AlphaBetaRobot:public QObject
{
	Q_OBJECT

public:
	AlphaBetaRobot();
	~AlphaBetaRobot();
	
	void SearchMain(void);
	int SearchFull(int vlAlpha, int vlBeta, int nDepth);
public slots:
	void getAiResponse(POINT*);
signals:
	void AIComplete(POINT*);//带AI的落子点返回,计算完毕
};