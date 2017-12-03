#pragma once
#include "StaticDefinition.h"
#include "StaticDefinition.h"

/*
 * 定义常量,结构体,枚举等
 */
enum Color
{
	space = -1, /*无棋子*/
	black = 0, /*黑棋*/
	white = 1  /*白棋*/
};

enum Player
{
	Human = 0,
	Ai = 1
};

enum ENTRY_TYPE
{
	exact,			/*精确*/
	lower_bound,	/*下界*/
	upper_bound		/*上界*/
};


//哈希表中元素的结构定义
typedef struct HASHITEM
{
	__int64 checksum;         //64位校验码
	ENTRY_TYPE entry_type;	  //数据类型
	short depth;			  //取得此值时的层次
	short eval;				  //节点的值
}HashItem;

typedef struct Node
{
	int x;
	int y;
}POINT;

//用以表示棋子位置的结构
typedef struct _stoneposition
{
	unsigned char x=-1;
	unsigned char y=-1;
}STONEPOS;

//typedef struct _movestone
//{
//	unsigned char nRenjuID;
//	POINT ptMovePoint;
//}MOVESTONE;

//这个结构用以表示走法

typedef struct _stonemove
{
	STONEPOS StonePos;//棋子位置
	int Score;         //走法的分数
}STONEMOVE;



#define GRID_NUM    15 //每一行(列)的棋盘交点数
#define GRID_COUNT  225//棋盘上交点总数
/*棋型*/
#define TOBEANALSIS 0  //尚未分析,正待分析的
#define Sleep2      1  //眠二
#define Sleep3      2  //眠三
#define Sleep4      3  //冲四/眠四
#define Alive2      4  //活二
#define Alive3      5  //活三
#define Alive4      6  //活四
#define Five        7  //五连
#define NOTYPE      11 //未定义
#define ANALSISED   255//已分析过的

#define MATE_VALUE 100000/*最大分数*/
#define Max_Level 4		 /*最大搜索深度*/
#define LIMIT_DEPTH 4   /*迭代加深最大深度*/
#define ADVANCED_VALUE 3 /*执黑先行奖励*/
//位置重要性价值表,此表从中间向外,越往外价值越低
const int PosValue[GRID_COUNT] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,
	0,1,2,3,3,3,3,3,3,3,3,3,2,1,0,
	0,1,2,3,4,4,4,4,4,4,4,3,2,1,0,
	0,1,2,3,4,5,5,5,5,5,4,3,2,1,0,
	0,1,2,3,4,5,6,6,6,5,4,3,2,1,0,
	0,1,2,3,4,5,6,7,6,5,4,3,2,1,0,
	0,1,2,3,4,5,6,6,6,5,4,3,2,1,0,
	0,1,2,3,4,5,5,5,5,5,4,3,2,1,0,
	0,1,2,3,4,4,4,4,4,4,4,3,2,1,0,
	0,1,2,3,3,3,3,3,3,3,3,3,2,1,0,
	0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
/*
 * 棋盘初始状态
 */
const int cucpcStartup[GRID_COUNT]=
{
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

typedef unsigned char       BYTE;