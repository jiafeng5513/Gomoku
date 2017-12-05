#include "AlphaBetaRobot.h"
#include <qDebug>


AlphaBetaRobot::AlphaBetaRobot()
{
	situation.Startup();
	for (int i=0;i<GRID_COUNT;i++)
	{
		Search.nHistoryTable[i] = 0;
	}
}


AlphaBetaRobot::~AlphaBetaRobot()
{
}


// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
int AlphaBetaRobot::SearchFull(int vlAlpha, int vlBeta, int nDepth) {
	//qDebug() << "---------->(" << vlAlpha << "," << vlBeta << "," << nDepth << ")<----------" << "call SearchFull";
	int pcCaptured;
	int vl, vlBest, mvBest;
	int mvs[GRID_COUNT];//可行解数量不可能多于棋盘上的角点数量
						// 一个Alpha-Beta完全搜索分为以下几个阶段
						// 1. 到达水平线，则返回局面评价值
	if (nDepth == 0) {
		int temp= situation.Evaluate();
		//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "ready to return by depth = 0,situtation.nDistance= " << situation.nDistance << "  Evaluate()= " << temp;
		return temp;
	}

	// 2. 初始化最佳值和最佳走法
	vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)
	mvBest = 0;           // 这样可以知道，是否搜索到了Beta走法或PV走法，以便保存到历史表

						  // 3. 生成全部走法，并根据历史表排序
	int nGenMoves = situation.GenerateMoves(mvs);
	qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
	//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "situation.GenerateMoves() = " << nGenMoves;
	// 4. 逐一走这些走法，并进行递归
	for (int i = 0; i < nGenMoves; i++) {
		situation.MakeMove(mvs[i]);
		vl = -SearchFull(-vlBeta, -vlAlpha, nDepth - 1);
		//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "  vl=  " << vl;
		situation.UndoMakeMove(mvs[i]);
		// 5. 进行Alpha-Beta大小判断和截断
		if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
			vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
			if (vl >= vlBeta) { // 找到一个Beta走法
				mvBest = mvs[i];  // Beta走法要保存到历史表
				/*Search.nHistoryTable[mvs[i]] += nDepth * nDepth;
				return vlBest;*/
				//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ") Beat-Cut,mvBest=   " << mvBest;
				break;            // Beta截断
			}
			if (vl > vlAlpha) { // 找到一个PV走法
				mvBest = mvs[i];  // PV走法要保存到历史表
				vlAlpha = vl;     // 缩小Alpha-Beta边界
				//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ") PV,mvBest=   " << mvBest;
			}
		}

	}

	// 5. 所有走法都搜索完了，把最佳走法(不能是Alpha走法)保存到历史表，返回最佳值
	if (vlBest == -MATE_VALUE) {
		// 如果是vlBest没有被赋值过(没有生成任何走法)
		//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ")" << "return by dead,value=  "<< situation.nDistance - MATE_VALUE;
		return situation.nDistance - MATE_VALUE;
	}
	//Search.nHistoryTable[mvBest] += nDepth * nDepth;
		
		
	if (mvBest != 0) {
		// 如果不是Alpha走法，就将最佳走法保存到历史表
		Search.nHistoryTable[mvBest] += nDepth * nDepth;
		if (situation.nDistance == 0) {
			//搜索根节点时，总是有一个最佳走法(因为全窗口搜索不会超出边界)，将这个走法保存下来
			Search.mvResult = mvBest;
			//qDebug() << "(" << vlAlpha << "," << vlBeta << "," << nDepth << ") update Search.mvResult to: " << mvBest;
		}
	}
	return vlBest;
}

// 迭代加深搜索过程
void AlphaBetaRobot::SearchMain(void) {
	int i, t, vl;

	// 初始化
	memset(Search.nHistoryTable, 0, 225 * sizeof(int)); // 清空历史表
	t = clock();       // 初始化定时器
	situation.nDistance = 0; // 初始步数
	//qDebug() << "======begin to Search by alpha beta======";
	//SearchFull(-100000, 100000, 4);
							 // 迭代加深过程
	for (i = 1; i <= LIMIT_DEPTH; i++) {
		vl = SearchFull(-MATE_VALUE, MATE_VALUE, i);
		 //叫杀终止
		if (vl > 100000 || vl < -100000) {//
			break;
		}
		 //超时终止
		if (clock() - t > CLOCKS_PER_SEC) {
			break;
		}
	}   // 深度耗尽终止
}

void AlphaBetaRobot::getAiResponse(POINT * playerPos)
{
	situation.MakeMove(playerPos->y*GRID_NUM + playerPos->x);
	//situation.ucpcSquares[playerPos->x*GRID_NUM + playerPos->y] = 0;//玩家默认落黑子
	//situation.ChangeSide();
	SearchMain();
	situation.MakeMove(Search.mvResult);
	qDebug() << "ai respone in :" << Search.mvResult;
	POINT *pos = new POINT;
	pos->y = Search.mvResult / GRID_NUM;
	pos->x = Search.mvResult - pos->y*GRID_NUM;
	emit AIComplete(pos);
}