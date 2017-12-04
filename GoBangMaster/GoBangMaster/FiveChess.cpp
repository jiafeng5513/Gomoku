#include "StdAfx.h"
#include "FiveChess.h"

#include<iostream>
#include<algorithm>
using namespace std;

FiveChess::FiveChess()
{
	InitChessMap();
	haveKinjite = false; //默认是无禁手
}

FiveChess::~FiveChess()
{
	chessPro.clear();
	vector< pair<int,int> >(chessPro).swap(chessPro); 
}

void FiveChess::InitChessMap()
{
	memset(chessMap,SPACE,sizeof(chessMap));
	chessPro.clear();
	currentX = -1; //初始位置
	currentY = -1;
	nCount = 0;
}

char FiveChess::GetStatus(int row,int col)
{
	return chessMap[row][col];
}


//设置有无禁手
void FiveChess::SetKinjite(bool isKinjite) 
{
	haveKinjite = isKinjite;
}

//设置当前落子点
void FiveChess::SetCurrentPoint(int row,int col)
{
	currentX = row;
	currentY = col;
}

//下一步棋
void FiveChess::SetChess(int row,int col,char chessFlag)
{
	chessMap[row][col] = chessFlag;
	++ nCount;
	chessPro.push_back(pair<int,int>(row,col));
	SetCurrentPoint(row,col);
	UpdateLimit(); //更新棋面限制框
}

//悔一步棋
void FiveChess::RetractChess()
{
	if(nCount >=2)
	{
		int x,y;
		for(int i=0;i<2;++i)
		{
			x = (chessPro.end()-1)->first;
			y = (chessPro.end()-1)->second;
			chessPro.pop_back();

			chessMap[x][y] = SPACE;
			-- nCount;
		}
	}
}

//判断chessFlag色的棋子是否赢棋
/*
    JudgeWin(int chessFlag)返回 chessFlag 说明chessFlag色的棋赢了，返回0表示
     	  尚未分出胜负，返回3，表示和棋。
    连续五个及五个以上的相同色的棋子相连即返回true，禁手在外层判断。
*/
char FiveChess::JudgeWin(char chessFlag)
{
	Search(currentX,currentY,chessFlag);

	if(changlian || cheng5)
		return chessFlag;
	
	if(nCount<15*15) //还不确定输赢，此时棋盘还未下满
	{
		return SPACE;
	}
	return PEACE_CHESS; //在判断了其他情况后还未返回就是和棋了
}


//返回1说明这点是黑棋的长连禁手，返回2是三三禁手，返回3是四四禁手，返回0表示不是禁手
int FiveChess::IsKinjite(int row,int col)
{
	//根据规则如果黑棋在形成五连的同时，又形成禁手，那么仍然是黑棋胜

	if(!haveKinjite)
		return 0;

	Search(row,col,BLACK_CHESS);
	
	if(changlian)
		return 1;
	if(cheng5)  // 
		return 0;
	if(lian3+tiao3>=2)
		return 2;
	if(huo4 + chong4 + tiaochong4 >= 2)
		return 3;

	return 0;
}

//更新棋面限制框
void FiveChess::UpdateLimit()
{
	int MM = 2;
	if(1 == nCount)
	{
		left = max(0,currentY-MM);
		right = min(SIZE-1,currentY+MM);
		top = max(0,currentX-MM);
		bottom = min(SIZE-1,currentX+MM);
	}
	else
	{
		if(currentX >= top+MM && currentX <= bottom-MM && 
			currentY >= left+MM && currentY <= right-MM )
			return ;
		else if(currentX >= top+MM && currentX <= bottom-MM)
		{
			if(currentY > right-MM)
				right = min(SIZE-1,currentY+MM);
			else if(currentY < left+MM)
				left = max(0,currentY-MM);
		}
		else if(currentY >= left+MM && currentY <=right-MM)
		{
			if(currentX > bottom-MM)
				bottom = min(SIZE-1,currentX+MM);
			else if(currentX < top+MM)
				top = max(0,currentX-MM);
		}
		else
		{
			if(currentX < top+MM && currentY > right-MM)
			{
				right = min(SIZE-1,currentY+MM);
				top = max(0,currentX-MM);
			}
			else if(currentX < top+MM && currentY < left+MM)
			{
				left = max(0,currentY-MM);
				top = max(0,currentX-MM);
			}
			else if(currentX > bottom-MM && currentY < left+MM)
			{
				left = max(0,currentY-MM);
				bottom = min(SIZE-1,currentX+MM);
			}
			else if(currentX > bottom-MM && currentY > right-MM)
			{
				right = min(SIZE-1,currentY+MM);
				bottom = min(SIZE-1,currentX+MM);
			}
		}
	}
}

/********对棋型的统计*******************************************/
// 尽量基于禁手规则，当然多重禁手不做处理。
// 另外在禁手规则下，白棋和黑棋的棋型判断不同，下面都会分别处理

// 假设在chessMap[row][col] 落子后,是否成5（或长连),返回1表示五连，返回2表示长连,否则返回0 
int FiveChess::GetCheng5ChangLian(int row,int col,char chessFlag)
{
	int left,right,i,j;
	// 横 
	for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
	for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	// 竖
	for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
	for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	// ' \ '
	for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
	for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	// ' / '
	for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
	for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
	
	if( 4 == left + right )
		return 1;
	else if( left + right >= 5 )
		return 2; 
	
	return 0; 
}

//  假设在chessMap[row][col] 落子后，返回构成活4的数量 
int FiveChess::GetHuo4(int row,int col,char chessFlag)
{
	int huo4=0,left,right,i,j;
	bool flag ;

	// 横 
	for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
	for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
	
	if( 3 == left + right && col-left-1 >= 0 && SPACE == chessMap[row][col-left-1]
		&& col+right+1 < SIZE && SPACE == chessMap[row][col+right+1] )
	{	
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] )
				flag = false;
			if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] )
				flag = false;
		}
		if( flag )
			++huo4;
	}

	// 竖
	for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
	for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
	
	if( 3 == left + right && row-left-1 >= 0 && SPACE == chessMap[row-left-1][col]
		&& row+right+1 < SIZE && SPACE == chessMap[row+right+1][col] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] )
				flag = false;
			if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] )
				flag = false;
		}
		if( flag )
			++huo4;
	}

	// ' \ '
	for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
	for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
	if( 3 == left + right && row-left-1 >= 0 && col-left-1 >= 0 && SPACE == chessMap[row-left-1][col-left-1]
		&& row+right+1 < SIZE && col+right+1 < SIZE && SPACE == chessMap[row+right+1][col+right+1])
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] )
				flag = false;
			if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] )
				flag = false;
		}
		if( flag )
			++huo4; 
	}

	// ' / '
	for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
	for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
	if( 3 == left + right && row-left-1 >= 0 && col+left+1 < SIZE && SPACE == chessMap[row-left-1][col+left+1]
		&& row+right+1 < SIZE && col-right-1 >= 0 && SPACE == chessMap[row+right+1][col-right-1] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] )
				flag = false;
			if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] )
				flag = false;
		}
		if( flag )
			++huo4;
	}

	return huo4;
}

	
// 假设在chessMap[row][col] 落子后，返回构成冲3和冲4(chong4,tiaochong4)的数量 
pair<int,pair<int,int> > FiveChess::GetChong3Chong4(int row,int col,char chessFlag)
{
	int chong3=0,chong4=0,tiaochong4=0,i,j,num1,num2; // num1,num2 记录两边空格的数量 
	int left1,left2 ,right1,right2; // left1,righ1 记录碰到空格之前的 chessFlag 数量，.... 
	
	// 横
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;

	for(i=col-1; i>=0; --i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[row][i-1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	 // 继续搜索另一边 
	for(i=col+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[row][i+1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}

	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}
	
	
	// 竖 
	num1 = num2 = 0;
	left1 = left2 = right1 =right2 = 0;
	for(i=row-1; i>=0; --i)
	{
		if( SPACE == chessMap[i][col] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[i-1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[i][col])
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[i+1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}
		
	// ' \ '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col-1; i>=0 && j>=0; --i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j-1 >= 0 && SPACE == chessMap[i-1][j-1] )
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1,j=col+1; i<SIZE && j<SIZE; ++i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j+1 < SIZE && SPACE == chessMap[i+1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}
	
	// ' / '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col+1; i>=0 && j<SIZE; --i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j+1 < SIZE && SPACE == chessMap[i-1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	} 
	for(i=row+1,j=col-1; i<SIZE && j>=0; ++i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j-1 >= 0 && SPACE == chessMap[i+1][j-1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( left1 + left2 + right1 >= 2 )
	{
		if( 0 == num1 && 2 == left1+right1 && num2 )
			++chong3;
		if( num1 && left1 < 2 && right1 < 2  && left2 )
			++chong3;
	}
	if( right1 + right2 + left1 >= 2 )
	{
		if( 0 == num2 && 2 == left1+right1 && num1)
			++chong3;
		if( num2 && left1 < 2 && right1 < 2 && right2 )
			++chong3;
	}
	if( left1 + left2 + right1 >= 3 )
	{
		if( 0 == num1 && 3 == left1+right1 && num2 )
			++chong4;
		if( num1 && left1 < 3 && right1 < 3 && left2 )
			++tiaochong4;
	}
	if( right1 + right2 + left1 >= 3 ) 
	{ 
		if( 0 == num2 && 3 == left1+right1 && num1 )
			++chong4;
		if( num2 && left1 < 3 && right1 < 3 && right2 )
			++tiaochong4;	
	}

	return pair<int,pair<int,int> >(chong3,pair<int,int>(chong4,tiaochong4));
}


// 假设在chessMap[row][col] 落子后，返回构成连2和连3的数量 
// 严格定义 "连三"，因为禁手规则需要，"连二"的话没必要非常严格。
pair<int,int> FiveChess::GetLian2Lian3(int row,int col,char chessFlag)
{
	int lian2=0,lian3=0,left,right,i,j,num1,num2;
	bool flag;
	
	// 横 
	for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
	for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
	
	if( 1 == left + right )
	{
		for(i=col-left-1, num1=0; i>=0 && SPACE == chessMap[row][i]; --i, ++num1);
		for(i=col+right+1, num2=0; i<SIZE && SPACE == chessMap[row][i]; ++i, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;
	}

	if( 2 == left + right )
	{
		flag = true;
		if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] ) // 如果满足这个就是冲四了
			flag = false;
		if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] ) // 如果满足这个就是冲四了
			flag = false;

		if( flag )
		{
			for(i=col-left-1, num1=0; i>=0 && SPACE == chessMap[row][i]; --i, ++num1);
			for(i=col+right+1, num2=0; i<SIZE && SPACE == chessMap[row][i]; ++i, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}	
	}
	
	// 竖
	for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
	for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
	
	if( 1 == left + right )
	{
		for(i=row-left-1, num1=0; i>=0 && SPACE == chessMap[i][col]; --i, ++num1);
		for(i=row+right+1, num2=0; i<SIZE && SPACE == chessMap[i][col]; ++i, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;	
	}
	if( 2 == left + right )
	{
		flag = true;
		if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] ) // 如果满足这个就是冲四了
			flag = false;
		if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] ) // 如果满足这个就是冲四了
			flag = false;

		if( flag )
		{
			for(i=row-left-1, num1=0; i>=0 && SPACE == chessMap[i][col]; --i, ++num1);
			for(i=row+right+1, num2=0; i<SIZE && SPACE == chessMap[i][col]; ++i, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}
	}
	
	// ' \ '
	for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
	for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
	
	if( 1 == left + right )
	{
		for(i=row-left-1, j=col-left-1, num1=0; i>=0 && j>=0 && SPACE == chessMap[i][j]; --i, --j, ++num1);
		for(i=row+right+1, j=col+right+1, num2=0; i<SIZE && j<SIZE && SPACE == chessMap[i][j]; ++i, ++j, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;	
	}
	if( 2 == left + right )
	{
		flag = true;
		if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] ) // 如果满足这个就是冲四了
			flag = false;
		if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] ) // 如果满足这个就是冲四了
			flag = false;

		if( flag )
		{
			for(i=row-left-1, j=col-left-1, num1=0; i>=0 && j>=0 && SPACE == chessMap[i][j]; --i, --j, ++num1);
			for(i=row+right+1, j=col+right+1, num2=0; i<SIZE && j<SIZE && SPACE == chessMap[i][j]; ++i, ++j, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}
	}
	
	// ' / '
	for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
	for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
	
	if( 1 == left + right )
	{
		for(i=row-left-1, j=col+left+1, num1=0; i>=0 && j<SIZE && SPACE == chessMap[i][j]; --i, ++j, ++num1);
		for(i=row+right+1, j=col-right-1, num2=0; i<SIZE && j>=0 && SPACE == chessMap[i][j]; ++i, --j, ++num2);
		if( num1 && num2 && num1 + num2 >= 4)
			++lian2;	
	}
	if( 2 == left + right )
	{
		flag = true;
		if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] ) // 如果满足这个就是冲四了
			flag = false;
		if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] ) // 如果满足这个就是冲四了
			flag = false;

		if( flag )
		{
			for(i=row-left-1, j=col+left+1, num1=0; i>=0 && j<SIZE && SPACE == chessMap[i][j]; --i, ++j, ++num1);
			for(i=row+right+1, j=col-right-1, num2=0; i<SIZE && j>=0 && SPACE == chessMap[i][j]; ++i, --j, ++num2);
			if( num1 && num2 && num1 + num2 >= 3)
				++lian3;
		}
	}
	
	return pair<int,int>(lian2,lian3);
}


// 假设在chessMap[row][col] 落子后，返回构成跳2和跳3的数量 
// 严格定义 "跳三"，因为禁手规则需要，跳二的话没必要非常严格。
pair<int,int> FiveChess::GetTiao2Tiao3(int row,int col,char chessFlag)
{
	int tiao2=0,tiao3=0,i,j,num1,num2;
	int left1,left2,right1,right2;  // 这里和 GetChong3Chong4() 中的含义 相同 
	bool flag;

	// 横
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	
	for(i=col-1; i>=0; --i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[row][i-1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	 // 继续搜索另一边 
	for(i=col+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[row][i] )
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[row][i+1])
				break;
		}
		else if( chessFlag == chessMap[row][i] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && col-left1-left2-2 >=0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row][col-left1-left2-2] && SPACE == chessMap[row][col+right1+1] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row][col-left1-1] && SPACE == chessMap[row][col+right1+right2+2] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && col-left1-left2-2 >=0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row][col-left1-left2-2] && SPACE == chessMap[row][col+right1+1] )	
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( col-left1-left2-3 >= 0 && chessFlag == chessMap[row][col-left1-left2-3] )
				flag = false;
			if( col+right1+2 < SIZE && chessFlag == chessMap[row][col+right1+2] )
				flag = false;	
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row][col-left1-1] && SPACE == chessMap[row][col+right1+right2+2] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( col-left1-2 >= 0 && chessFlag == chessMap[row][col-left1-2] )
				flag = false;
			if( col+right1+right2+3 < SIZE && chessFlag == chessMap[row][col+right1+right2+3] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	
	// 竖 
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1; i>=0; --i)
	{
		if( SPACE == chessMap[i][col] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && SPACE == chessMap[i-1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1; i<SIZE; ++i)
	{
		if( SPACE == chessMap[i][col])
		{
			++num2;
			if( num2 >= 2 )
				break;
			if(i+1 < SIZE && SPACE == chessMap[i+1][col])
				break;
		}
		else if( chessFlag == chessMap[i][col] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col] && SPACE == chessMap[row+right1+1][col] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col] && SPACE == chessMap[row+right1+right2+2][col] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col] && SPACE == chessMap[row+right1+1][col] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-left2-3 >= 0 && chessFlag == chessMap[row-left1-left2-3][col] )
				flag = false;
			if( row+right1+2 < SIZE && chessFlag == chessMap[row+right1+2][col] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col] && SPACE == chessMap[row+right1+right2+2][col] )
		
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-2 >= 0 && chessFlag == chessMap[row-left1-2][col] )
				flag = false;
			if( row+right1+right2+3 < SIZE && chessFlag == chessMap[row+right1+right2+3][col] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	 
	// ' \ '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col-1; i>=0 && j>=0; --i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j-1 >= 0 && SPACE == chessMap[i-1][j-1] )
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	}
	for(i=row+1,j=col+1; i<SIZE && j<SIZE; ++i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j+1 < SIZE && SPACE == chessMap[i+1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& col-left1-left2-2 >= 0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col-left1-left2-2] 
		&& SPACE == chessMap[row+right1+1][col+right1+1] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col-left1-1] 
		&& SPACE == chessMap[row+right1+right2+2][col+right1+right2+2] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && row+right1+1 < SIZE
		&& col-left1-left2-2 >= 0 && col+right1+1 < SIZE
		&& SPACE == chessMap[row-left1-left2-2][col-left1-left2-2] 
		&& SPACE == chessMap[row+right1+1][col+right1+1] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-left2-3 >= 0 && col-left1-left2-3 >= 0 && chessFlag == chessMap[row-left1-left2-3][col-left1-left2-3] )
				flag = false;
			if( row+right1+2 < SIZE && col+right1+2 < SIZE && chessFlag == chessMap[row+right1+2][col+right1+2] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && row+right1+right2+2 < SIZE
		&& col-left1-1 >= 0 && col+right1+right2+2 < SIZE
		&& SPACE == chessMap[row-left1-1][col-left1-1] 
		&& SPACE == chessMap[row+right1+right2+2][col+right1+right2+2] )	
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-2 >= 0 && col-left1-2 >= 0 && chessFlag == chessMap[row-left1-2][col-left1-2] )
				flag = false;
			if( row+right1+right2+3 < SIZE && col+right1+right2+3 < SIZE && chessFlag == chessMap[row+right1+right2+3][col+right1+right2+3] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	
	// ' / '
	num1 = num2 = 0;
	left1 = left2 = right1 = right2 = 0;
	for(i=row-1,j=col+1; i>=0 && j<SIZE; --i, ++j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num1;
			if( num1 >= 2 )
				break;
			if(i-1 >= 0 && j+1 < SIZE && SPACE == chessMap[i-1][j+1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num1 ? ++left2 : ++left1;
		else
			break;
	} 
	for(i=row+1,j=col-1; i<SIZE && j>=0; ++i, --j)
	{
		if( SPACE == chessMap[i][j] )
		{
			++num2;
			if( num2 >= 2)
				break;
			if(i+1 < SIZE && j-1 >= 0 && SPACE == chessMap[i+1][j-1])
				break;
		}
		else if( chessFlag == chessMap[i][j] )
			num2 ? ++right2 : ++right1;
		else
			break;
	}
	
	if( 1 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && col+left1+left2+2 < SIZE
		&& row+right1+1 < SIZE && col-right1-1 >= 0
		&& SPACE == chessMap[row-left1-left2-2][col+left1+left2+2] 
		&& SPACE == chessMap[row+right1+1][col-right1-1] )
		++tiao2;
	if( 1 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && col+left1+1 < SIZE
		&& row+right1+right2+2 < SIZE && col-right1-right2-2 >= 0
		&& SPACE == chessMap[row-left1-1][col+left1+1] 
		&& SPACE == chessMap[row+right1+right2+2][col-right1-right2-2] )
		++tiao2;

	if( 2 == left1+left2+right1 && 2 == num1 && row-left1-left2-2 >=0 && col+left1+left2+2 < SIZE
		&& row+right1+1 < SIZE && col-right1-1 >= 0
		&& SPACE == chessMap[row-left1-left2-2][col+left1+left2+2] 
		&& SPACE == chessMap[row+right1+1][col-right1-1] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-left2-3 >= 0 && col+left1+left2+3 < SIZE && chessFlag == chessMap[row-left1-left2-3][col+left1+left2+3] )
				flag = false;
			if( row+right1+2 < SIZE && col-right1-2 >= 0 && chessFlag == chessMap[row+right1+2][col-right1-2] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	if( 2 == right1+right2+left1 && 2 == num2 && row-left1-1 >= 0 && col+left1+1 < SIZE
		&& row+right1+right2+2 < SIZE && col-right1-right2-2 >= 0
		&& SPACE == chessMap[row-left1-1][col+left1+1] 
		&& SPACE == chessMap[row+right1+right2+2][col-right1-right2-2] )
	{
		flag = true;
		if( haveKinjite && BLACK_CHESS == chessFlag )
		{
			if( row-left1-2 >= 0 && col+left1+2 < SIZE && chessFlag == chessMap[row-left1-2][col+left1+2] )
				flag = false;
			if( row+right1+right2+3 < SIZE && col-right1-right2-3 >= 0 && chessFlag == chessMap[row+right1+right2+3][col-right1-right2-3] )
				flag = false;
		}
		if( flag )
			++tiao3;
	}
	return pair<int,int>(tiao2,tiao3);
}

// 小棋型的判断
void FiveChess::OntherChessType(int row,int col,char chessFlag)
{
	chong2 = huo1 = chong1 = 0; 
	int i,j,num[4],border[4][2];
	
	memset(num,0,sizeof(num));
	memset(border,1,sizeof(border));
	
	// 横
	for(i=col-1; i>=0; --i)
	{
		if( chessFlag == chessMap[row][i] )
		{ 
			++num[0];
			continue; 
		} 
		if( SPACE == chessMap[row][i] )
		{
			border[0][0] = 0;
			break;
		}
		else
		{
			border[0][0] = 1;
			break;
		}
	}
	for(i=col+1; i<SIZE; ++i)
	{
		if( chessFlag == chessMap[row][i] )
		{
			++num[0];
			continue;
		}
		if( SPACE == chessMap[row][i] )
		{
			border[0][1] = 0;
			break;
		}
		else
		{
			border[0][1] = 1;
			break;
		}
	}
	
	// 竖
	for(i=row-1; i>=0; --i)
	{
		if( chessFlag == chessMap[i][col] )
		{
			++num[1];
			continue;
		}	
		if( SPACE == chessMap[i][col] )
		{
			border[1][0] = 0;
			break;
		}
		else
		{
			border[1][0] = 1;
			break;
		}
	} 
	for(i=row+1; i<SIZE; ++i)
	{
		if( chessFlag == chessMap[i][col] )
		{
			++num[1];
			continue;
		}
		if( SPACE == chessMap[i][col] )
		{
			border[1][1] = 0;
			break;
		}
		else
		{
			border[1][1] = 1;
			break;
		}
	}
	
	// ' \ '
	for(i=row-1, j=col-1; i>=0 && j>=0; --i, --j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[2];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[2][0] = 0;
			break;
		}
		else
		{
			border[2][0] = 1;
			break;
		}
	}
	for(i=row+1, j=col+1; i<SIZE && j<SIZE; ++i, ++j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[2];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[2][1] = 0;
			break;
		}
		else
		{
			border[2][1] = 1;
			break;
		}
	}
	
	// ' / '
	for(i=row-1, j=col+1; i>=0 && j<SIZE; --i, ++j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[3];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[3][0] = 0;
			break;
		}
		else
		{
			border[3][0] = 1;
			break;
		}
	}
	for(i=row+1, j=col-1; i<SIZE && j>=0; ++i, --j)
	{
		if( chessFlag == chessMap[i][j] )
		{
			++num[3];
			continue;
		}
		if( SPACE == chessMap[i][j] )
		{
			border[3][1] = 0;
			break;
		}
		else
		{
			border[3][1] = 1;
			break;
		}
	}
	
	// 下面判断棋型 
	for(i=0; i<4; ++i)
	{
		if( 1 == num[i] && 1 == ( border[i][0] + border[i][1] ) ) 
			++chong2;
		else if( 0 == num[i] && 0 == ( border[i][0] + border[i][1] ) )
			++huo1;
		else if( 0 == num[i] && 1 == ( border[i][0] + border[i][1] ) )
			++chong1;
	} 
}

/***************************************************************/

//搜索 chessMap[row][col] 这点能够针对 chessFlag 色的棋能够构成哪些棋型
void FiveChess::Search(int row,int col,char chessFlag)
{
	//初始化
	changlian = cheng5 = huo4 = chong4 = tiaochong4 = lian3 = tiao3 = chong3 = lian2 = tiao2 = 0;
	int res;
	pair<int,int> pi;
	pair<int,pair<int,int> > pip;

	// 五连，长连
	res = GetCheng5ChangLian(row,col,chessFlag);
	if( 1 == res )
		cheng5 = 1;
	else if( 2 == res )
		changlian = 1;

	// 活四
	huo4 = GetHuo4(row,col,chessFlag);

	// 冲三，冲四
	pip = GetChong3Chong4(row,col,chessFlag);
	chong3 = pip.first;
	chong4 = pip.second.first;
	tiaochong4 = pip.second.second;

	// 活三： 包括 连三 和 跳三
	// 活二： 包括 连二 和 跳二 ，（ 这里还是把跳二 和连二 分开了，开局时需要）
	pi = GetLian2Lian3(row,col,chessFlag);
	lian2 = pi.first;
	lian3 = pi.second;

	pi = GetTiao2Tiao3(row,col,chessFlag);
	tiao2 = pi.first;
	tiao3 = pi.second;
}

// 通过给这个位置加分来实现 防守对方的进攻方向
// 原理：将分值与这个点周围有多少空格和对方的棋子挂钩
int FiveChess::AddScore(int row,int col,char chessFlag)
{
	int score = 0;
	int s = 0,y = 0,m = 0; 
	char other = (BLACK_CHESS == chessFlag ? WHITE_CHESS : BLACK_CHESS);

	for(int i= max(0,row-2); i<= min(SIZE-1,row+2); ++i)
	{
		for(int j= max(0,col-2); j<= min(SIZE-1,col+2); ++j)	
		{
			if(SPACE == chessMap[i][j])
				++s;
			else if( other == chessMap[i][j])
				++y;
			else
				++m;
		}
	}

	score = y*30 + s*10 + m*10;
	return score;
}
/*
     这样设计函数的话，就同时涵盖了禁手与非禁手。
*/
/*
	下面跳3和冲3，冲4的一种棋型的赋分理论上是有点不可避免的问题，看造化了
	跳冲4包含了冲3等.....
*/
int FiveChess::GetScore(int row,int col,char chessFlag,char flag)
{
	int score = 10;
	
	if( chessFlag == flag ) // 对 自己而言
	{
		if( BLACK_CHESS == chessFlag && IsKinjite(row,col) ) //是自己的禁手点 
			return -1;
		Search(row,col,chessFlag); //
		if(changlian || cheng5)
			score += 5000000;
		if(huo4)
			score += 250000; 
		if(chong4+tiaochong4 >= 2)
			score += 250000;
		if((lian3+tiao3) && (chong4+tiaochong4))
			score += 200000;
		if(lian3+tiao3 >= 2)
			score += 8000;

		score += chong4 *230; 
		score += tiaochong4*110;
		score += lian3 * 320;
		score += tiao3 * 150; //tiao3必定多加了一个tiao2的分
		score += lian2* 160;
		score += tiao2 * 150;
		
		if((chong4+tiaochong4+huo4) && (lian2+tiao2>=2) )
			score += 220;
		if((lian3+tiao3) && (lian2+tiao2>=2) )
			score += 200;
		if(chong3 && chong4)
			score += 100;
		if(chong3>=2 && tiaochong4)
			score += 100;
		if(chong3 >= 2)
			score += 100;
		if(chong3 && (lian2+tiao2))
			score += 50;
		
		score += chong3 * 40;
		score += chong1 * 20;
		score += huo1 * 10;
		score += chong2 * 20;
	}
	else    // 对 对方而言
	{
		if( BLACK_CHESS == chessFlag && IsKinjite(row,col) ) // 是对方的禁手点
			return -1;
		Search(row,col,chessFlag); //
		if(changlian || cheng5)
			score += 1000000;
		if(huo4)
			score += 45000;
		if(chong4+tiaochong4 >= 2)
			score += 45000;
		if((lian3+tiao3) && (chong4+tiaochong4))
			score += 40000;
		if(lian3+tiao3 >= 2)
			score += 1900;

		score += chong4 *230; 
		score += tiaochong4*110;
		score += lian3 * 320;
		score += tiao3 * 150; //tiao3必定多加了一个tiao2的分
		score += lian2* 160;
		score += tiao2 * 150;
		
		if((chong4+tiaochong4+huo4) && (lian2+tiao2>=2) )
			score += 220;
		if((lian3+tiao3) && (lian2+tiao2>=2) )
			score += 200;
		if(chong3 && chong4)
			score += 100;
		if(chong3>=2 && tiaochong4)
			score += 100;
		if(chong3 >= 2)
			score += 100;
		if(chong3 && (lian2+tiao2))
			score += 50;
		
		score += chong3 * 40;
		score += chong1 * 20;
		score += huo1 * 10;
		score += chong2 * 20;
	}

	if( score >= 1900 || isDefend )
		score += AddScore(row,col,chessFlag); //
	return score;
}

//得到 chessMap[row][col] 这点对于 chessFlag 色的棋的价值
int FiveChess::GetScoreForComputer(int row,int col,char chessFlag)
{
	return GetScore(row,col,chessFlag,cComputer);
}

//得到 chessMap[row][col] 这点对于 chessFlag 色的棋的价值
int FiveChess::GetScoreForPeople(int row,int col,char chessFlag)
{
	return GetScore(row,col,chessFlag,cPeople);
}


//得到计算机应该下棋的当前最佳位置
void FiveChess::ComputerPlay()
{
	pair<int,int>pi = GetCurrentMaxPoint(cComputer);
	comX = pi.first;
	comY = pi.second;
	int tempX = comX,tempY = comY;

	// 对于对方的 连3和跳3，到底是防守活4点，还是冲4点，这个需要进一步判断
	// 对于连3，有2个可能落子点，对于跳3，有3个可能落子点
	// 这里将冲 4 点加上一定的分值（45000）
	if( 2 == winPeople && ( !winComputer || winComputer>winPeople) )
	{
		if( GetHuo4(comX,comY,cPeople) ) //人构成活4
		{
			int score = GetScoreForComputer(comX,comY,cPeople); //得到分数
			score += GetScoreForComputer(comX,comY,cComputer);
			//////////////////////////////////////////////////////////////////////////
			int row = comX,col = comY;
			int i,j,left,right;
			bool flag;
			char chessFlag = cPeople;

			// 横 
			for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
			for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
			
			if( 3 == left + right && col-left-1 >= 0 && SPACE == chessMap[row][col-left-1]
				&& col+right+1 < SIZE && SPACE == chessMap[row][col+right+1] )
			{	
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] )
						flag = false;
					if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] )
						flag = false;
				}
				if( flag ) 
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row,col-left-1,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row,col+right+1,cComputer)+45000;

					if(s1>score)
					{
						tempX = row, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row, tempY = col+right+1, score = s2;
					}
				}
			}

			// 竖
			for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
			for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
			
			if( 3 == left + right && row-left-1 >= 0 && SPACE == chessMap[row-left-1][col]
				&& row+right+1 < SIZE && SPACE == chessMap[row+right+1][col] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] )
						flag = false;
					if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row-left-1,col,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row+right+1,col,cComputer)+45000;
	
					if(s1>score)
					{
						tempX = row-left-1, tempY = col, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col, score = s2;
					}
				}
			}

			// ' \ '
			for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
			for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col-left-1 >= 0 && SPACE == chessMap[row-left-1][col-left-1]
				&& row+right+1 < SIZE && col+right+1 < SIZE && SPACE == chessMap[row+right+1][col+right+1])
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] )
						flag = false;
					if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row-left-1,col-left-1,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row+right+1,col+right+1,cComputer)+45000;
					
					if(s1>score)
					{
						tempX = row-left-1, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col+right+1, score = s2;
					}
				}
			}
			
			// ' / '
			for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
			for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col+left+1 < SIZE && SPACE == chessMap[row-left-1][col+left+1]
				&& row+right+1 < SIZE && col-right-1 >= 0 && SPACE == chessMap[row+right+1][col-right-1] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] )
						flag = false;
					if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForComputer(row-left-1,col+left+1,cComputer)+45000;
					if( right )
						s2 = GetScoreForComputer(row+right+1,col-right-1,cComputer)+45000;
				
					if(s1>score)
					{
						tempX = row-left-1, tempY = col+left+1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col-right-1, score = s2;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
	comX = tempX;
	comY = tempY;
}


//得到人应该下棋的当前最佳位置
void FiveChess::PeoplePlay()
{
	pair<int,int>pi = GetCurrentMaxPoint(cPeople);
	peoX = pi.first;
	peoY = pi.second;	
	int tempX = peoX,tempY = peoY;

	// 对于对方的 连3和跳3，到底是防守活4点，还是冲4点，这个需要进一步判断
	// 对于连3，有2个可能落子点，对于跳3，有3个可能落子点
	// 这里将冲 4 点加上一定的分值（45000）
	if( 2 == winComputer && ( !winPeople || winPeople>winComputer) )
	{
		if( GetHuo4(peoX,peoY,cComputer) ) //计算机构成活4
		{
			int score = GetScoreForPeople(peoX,peoY,cComputer); //得到分数
			score += GetScoreForPeople(peoX,peoY,cPeople);
			//////////////////////////////////////////////////////////////////////////
			int row = peoX,col = peoY;
			int i,j,left,right;
			bool flag;
			char chessFlag = cComputer;

			// 横 
			for(i=col-1, left=0; i>=0 && chessFlag == chessMap[row][i]; --i, ++left);
			for(i=col+1, right=0; i<SIZE && chessFlag == chessMap[row][i]; ++i, ++right); 
			
			if( 3 == left + right && col-left-1 >= 0 && SPACE == chessMap[row][col-left-1]
				&& col+right+1 < SIZE && SPACE == chessMap[row][col+right+1] )
			{	
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( col-left-2 >= 0 && chessFlag == chessMap[row][col-left-2] )
						flag = false;
					if( col+right+2 < SIZE && chessFlag == chessMap[row][col+right+2] )
						flag = false;
				}
				if( flag ) 
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row,col-left-1,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row,col+right+1,cPeople)+45000;

					if(s1>score)
					{
						tempX = row, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row, tempY = col+right+1, score = s2;
					}
				}
			}

			// 竖
			for(i=row-1, left=0; i>=0 && chessFlag == chessMap[i][col]; --i, ++left); 
			for(i=row+1, right=0; i<SIZE && chessFlag == chessMap[i][col]; ++i, ++right);
			
			if( 3 == left + right && row-left-1 >= 0 && SPACE == chessMap[row-left-1][col]
				&& row+right+1 < SIZE && SPACE == chessMap[row+right+1][col] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && chessFlag == chessMap[row-left-2][col] )
						flag = false;
					if( row+right+2 < SIZE && chessFlag == chessMap[row+right+2][col] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row-left-1,col,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row+right+1,col,cPeople)+45000;

					if(s1>score)
					{
						tempX = row-left-1, tempY = col, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col, score = s2;
					}
				}
			}

			// ' \ '
			for(i=row-1, j=col-1, left=0; i>=0 && j>=0 && chessFlag == chessMap[i][j]; --i, --j, ++left);
			for(i=row+1, j=col+1, right=0; i<SIZE && j<SIZE && chessFlag == chessMap[i][j]; ++i, ++j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col-left-1 >= 0 && SPACE == chessMap[row-left-1][col-left-1]
				&& row+right+1 < SIZE && col+right+1 < SIZE && SPACE == chessMap[row+right+1][col+right+1])
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col-left-2 >= 0 && chessFlag == chessMap[row-left-2][col-left-2] )
						flag = false;
					if( row+right+2 < SIZE && col+right+2 < SIZE && chessFlag == chessMap[row+right+2][col+right+2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row-left-1,col-left-1,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row+right+1,col+right+1,cPeople)+45000;

					if(s1>score)
					{
						tempX = row-left-1, tempY = col-left-1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col+right+1, score = s2;
					}
				}
			}
			
			// ' / '
			for(i=row-1, j=col+1, left=0; i>=0 && j<SIZE && chessFlag == chessMap[i][j]; --i, ++j, ++left);
			for(i=row+1, j=col-1, right=0; i<SIZE && j>=0 && chessFlag == chessMap[i][j]; ++i, --j, ++right);
			if( 3 == left + right && row-left-1 >= 0 && col+left+1 < SIZE && SPACE == chessMap[row-left-1][col+left+1]
				&& row+right+1 < SIZE && col-right-1 >= 0 && SPACE == chessMap[row+right+1][col-right-1] )
			{
				flag = true;
				if( haveKinjite && BLACK_CHESS == chessFlag )
				{
					if( row-left-2 >= 0 && col+left+2 < SIZE && chessFlag == chessMap[row-left-2][col+left+2] )
						flag = false;
					if( row+right+2 < SIZE && col-right-2 >= 0 && chessFlag == chessMap[row+right+2][col-right-2] )
						flag = false;
				}
				if( flag )
				{
					int s1 = 0,s2 = 0;
					if( left )
						s1 = GetScoreForPeople(row-left-1,col+left+1,cPeople)+45000;
					if( right )
						s2 = GetScoreForPeople(row+right+1,col-right-1,cPeople)+45000;

					if(s1>score)
					{
						tempX = row-left-1, tempY = col+left+1, score = s1;
					}
					if(s2>score)
					{
						tempX = row+right+1, tempY = col-right-1, score = s2;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
	peoX = tempX;
	peoY = tempY;
}


//得到并返回当前价值最大点
pair<int,int> FiveChess::GetCurrentMaxPoint(char chessFlag)
{
	int row,col;
	memset(computer,0,sizeof(computer));
	memset(people,0,sizeof(people));

	for(row=0;row<SIZE;++row)
	{
		for(col=0;col<SIZE;++col)
		{
			if(SPACE == chessMap[row][col])
			{
				if(chessFlag == cComputer)
				{
					people[row][col] = GetScoreForComputer(row,col,cPeople);
					computer[row][col] = GetScoreForComputer(row,col,cComputer);
				}
				else 
				{
					people[row][col] = GetScoreForPeople(row,col,cPeople);
					computer[row][col] = GetScoreForPeople(row,col,cComputer);
				}
			}
		}
	}

	// 
	int cx,cy,px,py;
	int maxComputer = -10,maxPeople = -10;
	int maxScore = -10;
	int xx,yy;

	for(row=0;row<SIZE;++row)
	{
		for(col=0;col<SIZE;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			
			if(BLACK_CHESS == chessFlag) // 防止禁手点
			{
				if(cPeople == chessFlag && -1 == people[row][col])
					continue;
				if(cComputer == chessFlag && -1 == computer[row][col])
					continue;
			}

			if(people[row][col]>maxPeople)
			{
				maxPeople = people[row][col];
				px = row;
				py = col;
			}
			
			if(computer[row][col]>maxComputer)
			{
				maxComputer = computer[row][col];
				cx = row;
				cy = col;
			} 
			if(computer[row][col]+people[row][col]>maxScore)
			{
				maxScore = computer[row][col] + people[row][col];
				xx = row;
				yy = col;
			}
		}
	}

	winPeople = winComputer = mustWin = 0;
	if(chessFlag == cComputer)  ////站在计算机的角度
	{
		if(maxComputer >= 5000000)
			winComputer = 1;
		else if(maxComputer >= 200000)
			winComputer = 2;
		else if(maxComputer >= 8000)
			winComputer = 3;

		if(maxPeople >= 1000000)
			winPeople = 1;
		else if(maxPeople >= 40000)
			winPeople = 2;
		else if(maxPeople >= 1900)
			winPeople = 3;
	}
	else  
	{
		if(maxComputer >= 1000000)
			winComputer = 1;
		else if(maxComputer >= 40000)
			winComputer = 2;
		else if(maxComputer >= 1900)
			winComputer = 3;

		if(maxPeople >= 5000000)
			winPeople = 1;
		else if(maxPeople >= 200000)
			winPeople = 2;
		else if(maxPeople >= 8000)
			winPeople = 3;
	}

	if( 1 == winComputer || 1 == winPeople )
		mustWin = 1;
	else if( 2 == winComputer || 2 == winPeople )
		mustWin = 2;
	else if( 3 == winComputer || 3 == winPeople )
		mustWin = 3;

	//如果棋盘已满，那么 maxComputer 和 maxPeople 都应该为 -10 
	if(-10 == maxComputer && -10 == maxPeople)
		return pair<int,int>(-1,-1);

	// 针对 连3 ,到底是挡哪边
	if( mustWin ) //在有必赢点的时候取两者之和最高分，这样尽可能的避免失误
		return pair<int,int>(xx,yy);

	return maxComputer >= maxPeople ? pair<int,int>(cx,cy) : pair<int,int>(px,py);
}

//这个函数模拟计算机，只有一条叉
void FiveChess::InitGameTree(int row,int col,bool& flag,int depth,int& depthC,int& depthP,bool& stop,int depthRecord)
{
	if(stop || depth > DEPTH || depth > depthRecord+1) 
		return ;

	if(BLACK_CHESS == cComputer && IsKinjite(row,col)) // 计算机下了禁手位置,则人赢
	{
		depthP = depth;
		stop = true;
		return ;
	}
	chessMap[row][col] = cComputer;   //计算机下

	PeoplePlay();
	
	if(peoX != -1 && peoY != -1 ) //棋盘未满
	{
		if( 1 == winPeople) //人有成五点便马上停止深搜
		{
			depthP = depth;
			stop = true;
		}
		else if( BLACK_CHESS == cPeople && IsKinjite(peoX,peoY))//人下了禁手点，则计算机赢
		{
			flag = true;
			depthC = depth;
			stop = true; 
		}
		else
		{		
			int px = peoX,py = peoY;
			chessMap[px][py] = cPeople;  //人下

		    ComputerPlay();
		
			if( comX != -1 && comY != -1 ) //棋盘未满
			{
				if( 1 == winComputer)  //计算机有成五点，立即返回
				{
					flag = true;
					depthC = depth;
					stop = true;
				}
				else
					InitGameTree(comX,comY,flag,depth+1,depthC,depthP,stop,depthRecord); //递归
			}

			chessMap[px][py] = SPACE; // 恢复
		}
	}
	chessMap[row][col] = SPACE; // 恢复
}

//这个函数模拟人，类似 InitGameTree()
void FiveChess::AgainGameTree(int row,int col,int depth,int& depthC,int& depthP,bool& stop,int& depthRecord)
{
	// 这里的depth上限应该适当高些，防止进行vc攻击
	if(stop || depth > 12 || depth >= depthRecord) // 这里的depthCFromIGT的意义详见 PeopleAttack() 
		return ;
	
	if(BLACK_CHESS == cPeople && IsKinjite(row,col)) // 人下了禁手位置,则计算机赢
	{
		depthC = depth;
		stop = true;
		return ;
	}
	chessMap[row][col] = cPeople;   //人下
	
	ComputerPlay();
	
	if(comX != -1 && comY != -1 ) //棋盘未满
	{
		if( 1 == winComputer) //计算机有成五点便马上停止深搜
		{
			depthC = depth;
			stop = true;
		}
		else if( BLACK_CHESS == cComputer && IsKinjite(comX,comY))//计算机下了禁手点，则人赢
		{
			depthP = depth;
			stop = true;
		}
		else
		{
			//说明人是vc攻击，那么搜索的深度应该增加
			if( winPeople && ( !winComputer || winPeople<winComputer ) ) 
			{
				++depthRecord;
			}

			int cx = comX,cy = comY;
			chessMap[cx][cy] = cComputer;  //计算机下
			
			PeoplePlay();
			
			if( peoX != -1 && peoY != -1 ) //棋盘未满
			{
				if( 1 == winPeople)  //人有成五点，立即返回
				{
					depthP = depth;
					stop = true;
				}
				else
					AgainGameTree(peoX,peoY,depth+1,depthC,depthP,stop,depthRecord); //递归
			}
			
			chessMap[cx][cy] = SPACE; // 恢复
		}
	}
	chessMap[row][col] = SPACE; // 恢复
}

// 模拟计算机进行攻击，看计算机能否在 depthMM 步内击败人
bool FiveChess::ComputerAttack(int r,int c,int depthMM)
{
	chessMap[r][c] = cPeople; //人下了这点
	
	int depthRecord = depthMM;

	bool flag = false; //标记计算机是否能击败人
	int row,col;
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			if(BLACK_CHESS == cComputer && IsKinjite(row,col))  //计算机执黑，且这点为禁手点
				continue;
			
			bool stop = false;
			int depthC = 999,depthP = 999;
			
			depthRecord = depthMM; //
			
			InitGameTree(row,col,flag,0,depthC,depthP,stop,depthRecord);

			if(depthP <= depthC) // 人先赢
				continue;
			
			// 计算机能先赢，则跳出
			flag = true;
			col = right+1;
			row = bottom+1;
		}
	}
	
	chessMap[r][c] = SPACE; //恢复
	
	if(flag)
		return true;
	return false;
}

// 模拟人进行攻击，看人能否在 depthCFromIGT 步内击败计算机
bool FiveChess::PeopleAttack(int r,int c,int depthCFromIGT)
{
	chessMap[r][c] = cComputer; //计算机下了这点

	int depthRecord = depthCFromIGT; // depth from InitGameTree()

	bool flag = false; //标记人是否能击败计算机
	int row,col;
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			if(BLACK_CHESS == cPeople && IsKinjite(row,col))  //人执黑，且这点为禁手点
				continue;
			
			bool stop = false;
			int depthC = 999,depthP = 999;
			
			depthRecord = depthCFromIGT; //


			AgainGameTree(row,col,0,depthC,depthP,stop,depthRecord); 

			if(depthC <= depthP) // 计算机先赢
				continue;

			// 人能先赢，则跳出
			flag = true;
			col = right+1;
			row = bottom+1;
		}
	}

	chessMap[r][c] = SPACE; //恢复

	if(flag)
		return true;
	return false;
}


/*
     AI 的进攻性和防御都很优秀
	 调用了 VCAttack() 进行 VCF和VCT攻击后，一有先手，几乎比赢，
	 后面的代码几乎成了多余,当然得靠后面的代码创建先手机会
*/
void FiveChess::AI()
{
	isDefend = false;
	AIState = 0;

	ComputerPlay();
	int tempX = comX,tempY = comY; //

	if(LayOut()) // 刚开始时布局
		return ;

	if(1 == mustWin)  //有成五点
		return ;

	// AI 等级为 2 时才进行 VC 攻击 
	if(2 == grade && VCAttack()) // 可以进行 VC 攻击
		return ;

	ControlDepth(); // 设置递归深度

	int row,col,depthRecord = 999,maxScore = -999;
	
	// 若找到一个必赢点，便将其置为 true，否则需要检查人是否存在赢的情况，例如：
	// 这种情况：计算机落子后构成一个冲4和一个活3，然后人在挡冲4的时候形成了活4，这样人就先赢了
	// 所以不能单纯依赖于最大值去判断
	bool safeFlag = false; 

	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			if(BLACK_CHESS == cComputer && IsKinjite(row,col))  //计算机执黑，且这点为禁手点
				continue;
		
			bool flag = false,stop = false;
			int depthC = 999,depthP = 999;

			InitGameTree(row,col,flag,0,depthC,depthP,stop,depthRecord); 
		
			//由于是单叉树，那么depthC和depthP至少有一个是不变的，
			//depthP<=depthC则说明若按这条路径下计算机不能先赢
			if(depthP <= depthC) 
				continue;

			// 上面是在计算机的进攻蓝图中，下面这个函数模拟人进攻，看谁先赢，人先赢，则返回true
			// 注意参数导入
			if( PeopleAttack(row,col,depthC) ) //// 有了一步，防御能力大大提高
				continue;

			if(flag)  //计算机有必赢点，便将这点保存起来
			{
				// 需要考虑禁手情况,主要原因是因为三三禁手
				bool ff = false;
				if(BLACK_CHESS == cComputer && haveKinjite && depthC>=2)
				{
					chessMap[row][col] = cComputer;  //假设下了这点
					for(int i=top;i<=bottom;++i)
					{
						for(int j=left;j<=right;++j)
						{
							if( IsKinjite(i,j) ) // 产生了禁手点
							{
								ff = true;
								i = bottom+1;
								j= right+1;
							}
						}
					}
					chessMap[row][col] = SPACE; //恢复
				}
				if(ff)
					continue;

				if(depthC < depthRecord)  //选取深度小的
				{
					maxScore = GetScoreForComputer(row,col,cComputer);
					depthRecord = depthC;
					tempX = row;
					tempY = col;
				}
				else if(depthC == depthRecord) //深度一样时选取权值大的
				{
					int res = GetScoreForComputer(row,col,cComputer);
					if(res > maxScore)
					{
						maxScore = res;
						tempX = row;
						tempY = col;
					}
				}
				safeFlag = true; ///
			}
		}

	}

	comX = tempX;
	comY = tempY;

	if(!safeFlag) // 需要检查人是否能赢
	{ 
		PeoplePlay();
		bool flag = false;
		if( winPeople )  //人能赢，计算机则阻挡
		{
			if(BLACK_CHESS != cComputer ||  !IsKinjite(peoX,peoY)) // 不是计算机的禁手点
			{
				comX = peoX;
				comY = peoY;
			}
			else
				flag = true;
		}
		if( flag ) // 那么尽可能的拖延人赢，找一个能尽量最晚输的点落子
		{ 
			int depthWin=8; //
			maxScore = -999;
			
			for(row=top;row<=bottom;++row)
			{
				for(col=left;col<=right;++col)
				{
					if(chessMap[row][col] != SPACE)
						continue;
					if( BLACK_CHESS == cComputer && IsKinjite(row,col)) //计算机不能下自己的禁手点
						continue;
					
					if(!PeopleAttack(row,col,depthWin)) // 人在深度 depthWin 内 不能先赢
					{
						// 那就找一个对计算机而言价值多大的点
						int res = GetScoreForComputer(row,col,cComputer);
						if(res > maxScore)
						{
							maxScore = res;
							tempX = row;
							tempY = col;
						}
					}
				}
			}
			
			comX = tempX;
			comY = tempY;
		}
				
	}

}


// 判断局势，从而设置 DEPTH
// 这里判断的局势是针对防守而言,比如看玩家落子后可构成多少活2，冲3，冲4等
// 当对方进攻不积极的时候，这时计算机就应该主动进攻
void FiveChess::ControlDepth()
{
	DEPTH = 12; //默认 
	
	if(nCount < 6)
	{
		DEPTH = 4;
		return ;
	}

	return ; // 进攻好了，防守个毛
/*
	int row,col;

	// people
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			Search(row,col,cPeople);

			if(lian2+tiao2>=3)
				DEPTH = min(DEPTH,7);

			if(chong4)
				DEPTH = min(DEPTH,6);
			if(lian3+tiao3)
				DEPTH = min(DEPTH,6);
			
			if(lian2+tiao2 && lian3+tiao3)
				DEPTH = min(DEPTH,5);
			if(chong4 && chong3 && lian2+tiao2)
				DEPTH = min(DEPTH,5);
			if(chong4 && chong3>=2)
				DEPTH = min(DEPTH,5);
		}
	}

	// computer
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if(chessMap[row][col] != SPACE)
				continue;
			Search(row,col,cComputer);
			
			if(chong4 && chong3) 
				++DEPTH;
			if(chong4 && lian2+tiao2)
				++DEPTH;
			if(lian3+tiao3 && chong3)
				++DEPTH;
		}
	}

	DEPTH = min(DEPTH,10);
*/

}

// 布局
//先手要攻，后手要守
/*
	二十六种开局中有两个必胜开局和两个必败开局：（开局是针对先手而言的）
	必胜开局是花月局和浦月局；
	必败开局是游星局和彗星局。
*/
bool FiveChess::LayOut()
{
	// 只考虑前面三步的布局
	if(nCount >= 3)
		return false;

	if(BLACK_CHESS == cComputer)// 计算机执黑，只考虑第三手
	{
		// 计算机第一手必然在天元，那么紧邻有8种位置，其他位置不考虑。
		int res = 0;
		srand((unsigned)time(NULL));
		res = rand() % 2;

		// 花月
		if( (6 == currentX || 8 == currentX) && 7 == currentY )
			comX = currentX, comY = ( res ? 6 : 8) ;
		else if( 7 == currentX && (6 == currentY || 8 == currentY) )
			comX = (res ? 6 : 8), comY = currentY;
		
		// 浦月
		else if( 6 == currentX && 8 == currentY )
			res ? ( comX = comY = 8 ) : ( comX = comY = 6 );
		else if( 6 == currentX && 6 == currentY )
			res ? ( comX = 8, comY = 6 ) : ( comX = 6, comY = 8 );
		else if( 8 == currentX && 6 == currentY )
			res ? ( comX = comY = 6 ) : ( comX = comY = 8 );
		else if( 8 == currentX && 8 == currentY )
			res ? ( comX = 8, comY = 6 ) : ( comX = 6, comY = 8 );

		// 若不是上面两种棋型,则判断白棋的相对位置
		else
		{
			if(currentX<=7 && currentY<=7)
				comX = comY = 8;
			else if(currentX>=7 && currentY>=7)
				comX = comY = 6;
			else if(currentX>=7 && currentY<=7)
				comX = 6, comY = 8;
			else
				comX = 8, comY = 6;
		}

	}
	else //计算机执白，只考虑第二手
	{
		// 四种情况，随机
		int res = 0;
		srand((unsigned)time(NULL));
		res = rand() % 4;

		while(1)
		{
			if( 0 == res )
			{
				if( currentX-1 >= 0 )
				{
					comX = currentX-1, comY = currentY;
					break;
				}
				else 
					res = 1;
			}
			if( 1 == res )
			{
				if( currentX+1 < SIZE )
				{
					comX = currentX+1, comY = currentY;
					break;
				}
				else
					res = 2;
			}
			if( 2 == res )
			{
				if( currentY-1 >= 0 )
				{
					comX = currentX, comY = currentY-1;
					break;
				}
				else
					res = 3;
			}
			if( 3 == res )
			{
				if( currentY+1 < SIZE )
				{
					comX = currentX, comY = currentY+1;
					break;
				}
				else
					res = 0;
			}
		}

	}
	return true;
}

// VC 攻击树
void FiveChess::VCAttackTree(int type,int row,int col,char cOneself,char cOpposite,int depth,bool& flag,int& ansDepth,int depthRecord)
{
	// 深度控制,深度太深的话，复杂度就太高了，depthRecord用来记录已经搜过的点中达必赢点的最小深度
	if( depth>depthMM || depth>depthRecord )
		return ;

	if(cOneself == cComputer)
	{
		ComputerPlay();
		if( 1 == winComputer )
			flag = true;
	}
	else
	{
		PeoplePlay();
		if( 1 == winPeople )
			flag = true;
	}
	if( flag ) // flag 为 true 时，表示可以必赢，能必赢则返回，不需要去找最优的，否则太慢
	{
		ansDepth = depth; //记录多少步能赢
		return ;
	}

	chessMap[row][col] = cOneself; // 自己落子

	int oppositeX,oppositeY;
	int winOpposite = 0;
	int dd = 1; // 深度的递增值
	
	//////////////////////对方思考/////////////////////////////////////////////
	if( cOpposite == cPeople ) 
	{
		PeoplePlay();
		oppositeX = peoX, oppositeY = peoY;
		winOpposite = winPeople;
	}
	else //
	{
		ComputerPlay();
		oppositeX = comX, oppositeY = comY;
		winOpposite = winComputer;
	}

	// 如果对方在防守的同时 也构成了冲4或活3，那么自己又需要反过来防守对方，此时应该将深度加1
	if( 1 == type ) // VCF 攻击
	{
		pair<int,pair<int,int> > ppi = GetChong3Chong4(oppositeX,oppositeY,cOpposite);
		if(ppi.second.first + ppi.second.second) //构成冲4或跳冲4
			dd = 2;

//		if(ppi.second.first + ppi.second.second >=2 ) //
//			winOpposite = 1;
	}
	else if( 2 == type ) // VCF 和 VCT 联合攻击
	{
		pair<int,pair<int,int> > ppi = GetChong3Chong4(oppositeX,oppositeY,cOpposite);
		if(ppi.second.first + ppi.second.second) //构成冲4或跳冲4
			dd = 2;
		pair<int,int> pi1 = GetLian2Lian3(oppositeX,oppositeY,cOpposite);
		if(pi1.second) //构成连3
			dd = 2;
		pair<int,int> pi2  = GetTiao2Tiao3(oppositeX,oppositeY,cOpposite);
		if(pi2.second) //构成跳3
			dd = 2;

//		if(ppi.second.first+ppi.second.second + pi1.second+pi2.second >=2 ) //
//			winOpposite = 1;
	}
	//////////////////////////////////////////////////////////////////////////

	if( (oppositeX != -1 && oppositeY != -1) && (winOpposite != 1) ) // 棋盘未满 且 对方 未成五
	{
		if( cOpposite == BLACK_CHESS && IsKinjite(oppositeX,oppositeY) ) // 该点是对方的禁手
			flag = true;
		else
		{
			chessMap[oppositeX][oppositeY] = cOpposite; // 对方落子

			VCPoint vcPoint[100];// 保存活3，冲4，活4这些点
			int cnt=0; 
			int i,j;

			for(i=0;i<SIZE;++i)
			{
				for(j=0;j<SIZE;++j)
				{
					if( chessMap[i][j] != SPACE )
						continue;
					if( BLACK_CHESS == cOneself && IsKinjite(i,j) ) // 自己的禁手点
						continue;

					int res = IsCombForVC(i,j,cOneself); 
					//是攻击点则保存,攻击类型分2种，VCF，和VCF、VCT联合攻击
					if( 1 == type && res<3 ) 
						continue;
					if( 2 == type && !res )
						continue;

					vcPoint[cnt++].init(i,j,res);
					if( cnt >= 100 )
					{
						i = j = SIZE;
					}
				}
			}
			
			sort(vcPoint,vcPoint+cnt);  //按优先级排序

			for(i=0;i<cnt;++i)
			{
				if( !flag )
				{
					VCAttackTree(type,vcPoint[i].row,vcPoint[i].col,cOneself,cOpposite,depth+dd,flag,ansDepth,depthRecord); //
				}
			}

			chessMap[oppositeX][oppositeY] = SPACE; // 恢复
		}
	}

	chessMap[row][col] = SPACE; //恢复
}


// 判断是否有可以进行 VC 攻击的点
// 先判断计算机能否 VCF攻击取胜，再判断人是否通过VCF攻击取胜，然后再判断计算机是否通过VCF和VCT联合
//攻击取胜，也判断人的VCF和VCT联合攻击(虽然慢一点).
// 为了提高 棋力， 不断的尝试各个深度，先从小的深度开始搜素，这样就能保证以最小的步数赢棋，
// 一旦找到必赢点，立即返回
// 目前仍然有bug，很难在速度和棋力之间权衡
bool FiveChess::VCAttack()
{
	// VC 攻击，活4，冲4(冲4和跳冲4)，活3（连3和跳3），优先级依次降低

	int row,col;
	VCPoint vcPoint[100]; //
	int cnt;
	int tempDepth; //深度控制

	VCPoint dePoint[100]; //用于 VC 防守，在下面的模拟人的攻击中保存攻击点
	int num = 0; //

	int tempX,tempY,depthRecord=9999,maxScore = -9999;
	bool ff = false;

	// 先判断计算机是否可进行 VCF 攻击,  可以则返回
	//////////////////////////////////////////////////////////////////////////
	cnt = 0;

	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if( chessMap[row][col] != SPACE )
				continue;
			if( BLACK_CHESS == cComputer && IsKinjite(row,col) ) // 计算机的禁手点
				continue;
			
			int res = IsCombForVC(row,col,cComputer);

			if( res<3 ) // VCF攻击
				continue;

			vcPoint[cnt++].init(row,col,res);	//是进攻点则先保存
			
			if(cnt>=100) 
			{
				row = bottom+1;
				col = right+1;
			}
		}
	}

	sort(vcPoint,vcPoint+cnt); //

	for(tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//深度不断递增
	{
		depthMM = tempDepth; //
		for(int i=0;i<cnt;++i)
		{
			bool flag = false;
			int ansDepth = 9999;
			VCAttackTree(1,vcPoint[i].row,vcPoint[i].col,cComputer,cPeople,0,flag,ansDepth,depthRecord);
			
			if(flag) // 找到一个必赢攻击点
			{	
				ff = true; //
				if(ansDepth<depthRecord) //选取最快能赢的
				{
					depthRecord = ansDepth;
					tempX = vcPoint[i].row;
					tempY = vcPoint[i].col;
					maxScore = GetScoreForComputer(tempX,tempY,cComputer);
					maxScore += GetScoreForComputer(tempX,tempY,cPeople);
				}
				else if( ansDepth == depthRecord ) //深度一样时选取权值大的
				{
					int res = GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cComputer);
					res += GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cPeople);
					if( res > maxScore )
					{
						maxScore = res;
						tempX = vcPoint[i].row;
						tempY = vcPoint[i].col;
					}
				}
			}
		}
		if( ff )
		{
			comX = tempX;
			comY = tempY; 
			AIState = 1; //
			return true;
		}

	}

	// 再判断人是否通过 VCF攻击取胜，若可以，则计算机不进行 VCF和VCT的联合攻击，因为会先输
	// 但还要继续判断 人是否还可以 通过 VCF和VCT的联合攻击取胜，这关系到计算机的落子
	int peoIsVCF = false;
	//////////////////////////////////////////////////////////////////////////
	cnt = 0;

	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if( chessMap[row][col] != SPACE )
				continue;
			if( BLACK_CHESS == cPeople && IsKinjite(row,col) ) // 人的禁手点
				continue;
			
			int res = IsCombForVC(row,col,cPeople);
			
			if( res<3 ) // VCF攻击
				continue;
			
			vcPoint[cnt++].init(row,col,res);	//是进攻点则先保存
			
			if(cnt>=100) 
			{
				row = bottom+1;
				col = right+1;
			}
		}
	}
	
	sort(vcPoint,vcPoint+cnt); //

	
	for(tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//深度不断递增
	{
		depthMM = tempDepth; //
		for(int i=0;i<cnt;++i)
		{
			bool flag = false;
			int ansDepth = 9999;

			VCAttackTree(1,vcPoint[i].row,vcPoint[i].col,cPeople,cComputer,0,flag,ansDepth,depthRecord);
			
			if(flag) // 找到一个必赢攻击点
			{
				ff = true; //
				i = cnt;
				tempDepth = VCDE_DEPTH_END+1;
			}
		}		
	}
	if( ff ) 
	{
		peoIsVCF = true;
		AIState = 2;
	}

	ff = false;
	
	//判断计算机能否通过 VCF和VCT联合攻击取胜,  前提是人不能进行 VCF攻击
	//////////////////////////////////////////////////////////////////////////
	if(!peoIsVCF)
	{
		cnt = 0;

		for(row=top;row<=bottom;++row)
		{
			for(col=left;col<=right;++col)
			{
				if( chessMap[row][col] != SPACE )
					continue;
				if( BLACK_CHESS == cComputer && IsKinjite(row,col) ) // 计算机的禁手点
					continue;
				
				int res = IsCombForVC(row,col,cComputer);
				
				if( !res ) // VCF和VCT联合攻击
					continue;
				
				vcPoint[cnt++].init(row,col,res);	//是进攻点则先保存
				
				if(cnt>=100) 
				{
					row = bottom+1;
					col = right+1;
				}
			}
		}
		sort(vcPoint,vcPoint+cnt); //

		for(tempDepth = VCDEPTH; tempDepth <= VCDEPTH_END; tempDepth += VCDEPTH_DIS)//深度不断递增
		{
			depthMM = tempDepth; //
			for(int i=0;i<cnt;++i)
			{
				bool flag = false;
				int ansDepth = 9999;
				VCAttackTree(2,vcPoint[i].row,vcPoint[i].col,cComputer,cPeople,0,flag,ansDepth,depthRecord);
				
				if(flag) //找到一个必赢攻击点
				{
					ff = true; //
					if(ansDepth<depthRecord) //选取最快能赢的
					{
						depthRecord = ansDepth;
						tempX = vcPoint[i].row;
						tempY = vcPoint[i].col;
						maxScore = GetScoreForComputer(tempX,tempY,cComputer);
						maxScore += GetScoreForComputer(tempX,tempY,cPeople);
					}
					else if( ansDepth == depthRecord ) //深度一样时选取权值大的
					{
						int res = GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cComputer);
						res += GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cPeople);
						if( res > maxScore )
						{
							maxScore = res;
							tempX = vcPoint[i].row;
							tempY = vcPoint[i].col;
						}
					}

				}
			}
			if( ff )
			{
				comX = tempX;
				comY = tempY; 
				AIState = 3;
				return true;
			}
		}
	}

	//判断人能否通过 VCF和VCT联合攻击取胜，  VC 防御
	//////////////////////////////////////////////////////////////////////////
	cnt = 0;
	depthRecord=9999;
	
	for(row=top;row<=bottom;++row)
	{
		for(col=left;col<=right;++col)
		{
			if( chessMap[row][col] != SPACE )
				continue;
			if( BLACK_CHESS == cPeople && IsKinjite(row,col) ) // 自己的禁手点
				continue;
			
			int res = IsCombForVC(row,col,cPeople);
			
			if( !res ) // VCF和VCT联合攻击
				continue;
			
			vcPoint[cnt++].init(row,col,res);	//是进攻点则先保存
			
			if(cnt>=100) 
			{
				row = bottom+1;
				col = right+1;
			}
		}
	}
	
	sort(vcPoint,vcPoint+cnt); //

	for(tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//深度不断递增
	{
		depthMM = tempDepth; //
		for(int i=0;i<cnt;++i)
		{
			bool flag = false;
			int ansDepth = 9999;
			isDefend = false;

			VCAttackTree(2,vcPoint[i].row,vcPoint[i].col,cPeople,cComputer,0,flag,ansDepth,depthRecord);
		
			if(flag) // 找到一个必赢攻击点
			{
				isDefend = true; //防守，主要是为了 防在对方进攻的方向
				ff = true; //		
				if(ansDepth<depthRecord) //选取最快能赢的
				{
					depthRecord = ansDepth;
					tempX = vcPoint[i].row;
					tempY = vcPoint[i].col;	
					maxScore = GetScoreForComputer(tempX,tempY,cPeople);
					maxScore += GetScoreForComputer(tempX,tempY,cComputer);
				}
				else if( ansDepth == depthRecord ) //深度一样时选取权值大的
				{
					int res = GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cPeople);
					res += GetScoreForComputer(vcPoint[i].row,vcPoint[i].col,cComputer);
					if( res > maxScore )
					{
						maxScore = res;
						tempX = vcPoint[i].row;
						tempY = vcPoint[i].col;
					}
				}
			}
		}
		if( ff ) 
		{
			// 防守，计算机选择攻击位置落子，落子条件为下了该点后使人不会产生 VC 攻击点
			//////////////////////////////////////////////////////////////////////////
			for(row=top;row<=bottom;++row)
			{
				for(col=left;col<=right;++col)
				{
					if(chessMap[row][col] != SPACE)
						continue;
					if(BLACK_CHESS == cComputer && IsKinjite(row,col))
						continue;
					
					int res1 = IsCombForVC(row,col,cComputer);
					int res2 = IsCombForVC(row,col,cPeople);

					if( !res1 && !res2)
						continue;
					else if( (3 == res1 || 4 == res1) && !res2 ) //一般情况下不要下自己冲4和跳冲4
						continue;

					int res = GetScoreForComputer(row,col,cComputer);
					res += GetScoreForComputer(row,col,cPeople);

					dePoint[num++].init(row,col,res);
				}
			}

			sort(dePoint,dePoint+num); //以权值排序

			for(int p=0;p<num;++p)
			{
				bool noVC = true;
	
				chessMap[dePoint[p].row][dePoint[p].col] = cComputer; //假设计算机在这落子
				
				// 下面依旧是模拟 人的 VC 攻击
				cnt = 0;
				depthRecord=9999;

				for(row=top;row<=bottom;++row)
				{
					for(col=left;col<=right;++col)
					{
						if( chessMap[row][col] != SPACE )
							continue;
						if( BLACK_CHESS == cPeople && IsKinjite(row,col) ) // 人的禁手点
							continue;
						
						int res = IsCombForVC(row,col,cPeople);
						
						if( !res ) // VCF和VCT联合攻击
							continue;
						
						vcPoint[cnt++].init(row,col,res);	//是进攻点则先保存
						
						if(cnt>=100) 
						{
							row = bottom+1;
							col = right+1;
						}
					}
				}
				
				sort(vcPoint,vcPoint+cnt); //
				
				
				for(tempDepth = VCDE_DEPTH; tempDepth <= VCDE_DEPTH_END; tempDepth += VCDE_DEPTH_DIS)//深度不断递增
				{
					depthMM = tempDepth; //
					for(int i=0;i<cnt;++i)
					{
						bool flag = false;
						int ansDepth = 9999;
						VCAttackTree(2,vcPoint[i].row,vcPoint[i].col,cPeople,cComputer,0,flag,ansDepth,depthRecord);
						
						if(flag) // 找到便立即跳出
						{
							noVC = false;
							i = cnt;
							tempDepth = VCDE_DEPTH_END+1;
						}
					}
				}
				
				chessMap[dePoint[p].row][dePoint[p].col] = SPACE; //恢复
				
				if(noVC) //如果人没有 VC 攻击点
				{
					tempX = dePoint[p].row;
					tempY = dePoint[p].col;
					break;
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			comX = tempX;
			comY = tempY; 
			AIState = 4;
			return true;
		}
	}
	return false;
}

//判断是否是 VC 攻击的攻击点，返回其优先级
int FiveChess::IsCombForVC(int row,int col,char chessFlag)
{
	Search(row,col,chessFlag);
	
	if(huo4)
		return 5;
	if(chong4)
		return 4;
	if(tiaochong4)
		return 3;
	if(lian3)
		return 2;
	if(tiao3)
		return 1;

	return 0;
}