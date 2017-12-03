#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGomoku.h"
#include "BoardScene.h"
#include "StaticDefinition.h"
#include "Alpha-Beta.h"
#include "Robot.h"


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
	//AiAgent *m_ai;
	Robot * robot;
public:
	Color HumanColor = black;	//人类执何种颜色棋子
	Color AiColor = white;		//Ai执何种颜色棋子
	Player CurrentTurn = Human; //当前轮到谁落子
	Color BoardMap[15][15];		//保存棋局,这个变量只用来在玩家落子的时候判断是否合法,和判断游戏是否结束

private:
	void UpdateWholeBoardView();//根据当前棋局数组刷新整个画面
	void PutDownStone(int x,int y);//下棋
	bool isGameOver(int x,int y);//判断游戏是否因为刚刚落下的这个棋子而结束
public slots:
	void OnMouseReleaseOnBoardSense(QPoint *pos);//鼠标在棋盘上空释放
	void OnAiComplete(POINT * pos);
signals:
	void IsTimeForAiToCalculate(POINT*);//发出时带人类的落子点
};
