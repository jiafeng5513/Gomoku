#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGomoku.h"
#include "BoardScene.h"
#include "StaticDefinition.h"
#include "AlphaBetaRobot.h"

/*
 *
 */
class QtGomoku : public QMainWindow
{
	Q_OBJECT

public:
	QtGomoku(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGomokuClass ui;
	BoardScene * m_scene;
	QPixmap * WhiteStone;
	QPixmap * BlackStone;
	QPixmap * MarkForLastStep;
	QGraphicsItem * Mark;
	QGraphicsTextItem *txtitem;
	//AiAgent *m_ai;
	AlphaBetaRobot * robot;
	int Count;					   //即将落下的棋子是第几个
	bool isPVE = true;			   //游戏模式是否为人机
	bool isHumanGetBlack = true;   //玩家是否选择执黑先行
	bool isEnableForbidden = false;//是否启动禁手
	bool m_isGameOver = true;	   //游戏是否结束
public:
	Color HumanColor = black;	//人类执何种颜色棋子
	Color AiColor = white;		//Ai执何种颜色棋子
	Player CurrentTurn = Human; //人机模式下当前轮到谁落子
	Color CurrentColor = black; //人人模式下当前轮到哪种颜色落子
	Color BoardMap[15][15];		//保存棋局,这个变量只用来在玩家落子的时候判断是否合法,和判断游戏是否结束

private:
	void UpdateWholeBoardView();//根据当前棋局数组刷新整个画面
	void PutDownStone(int x,int y);//下棋
	bool isGameOver();//判断游戏是否结束
signals:
	void IsTimeForAiToCalculate(POINT*);//发出时带人类的落子点
public slots:
	void OnMouseReleaseOnBoardSense(QPoint *pos);//鼠标在棋盘上空释放
	void OnAiComplete(POINT * pos);				 //响应Ai线程计算完毕
	void OnNewGame();							 //按键:新游戏
	void OnUndo();								 //按键:悔棋
	void OnStopGame();							 //按键:结束游戏
	void OnHelp();								 //按键:帮助
	void OnCheckedHumanBlack();					 //单选:玩家执黑
	void OnCheckedHumanWhite();				     //单选:玩家执白
	void OnCheckedEnableForbidden();			 //单选:启动禁手
	void OnCheckedUnableForbidden();			 //单选:关闭禁手
	void OnCheckedPVE();						 //单选:人机对战
	void OnCheckedPVP();						 //单选:人人对战
	void OnAiLevelChanged(int);					 //编辑:Ai等级变动
};
