#pragma once

/*
 * 定义常量,结构体,枚举等
 */
enum Color
{
	space = -1,/*无棋子*/
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
	unsigned char x;
	unsigned char y;
}STONEPOS;

typedef struct _movestone
{
	unsigned char nRenjuID;
	POINT ptMovePoint;
}MOVESTONE;

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
#define STWO        1  //眠二
#define STHREE      2  //眠三
#define SFOUR       3  //冲四
#define TWO         4  //活二
#define THREE       5  //活三
#define FOUR        6  //活四
#define FIVE        7  //五连
#define NOTYPE      11 //未定义
#define ANALSISED   255//已分析过的