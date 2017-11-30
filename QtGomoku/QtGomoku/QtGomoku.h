#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGomoku.h"
#include "BoardScene.h"
#include "StaticDefinition.h"
#include "Alpha-Beta.h"


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
	AiAgent *m_ai;
public:
	Color HumanColor = black;	//人类执何种颜色棋子
	Color AiColor = white;		//Ai执何种颜色棋子
	Player CurrentTurn = Human; //当前轮到谁落子
	Color BoardMap[15][15];		//保存棋局

private:
	void UpdateWholeBoardView();//根据当前棋局数组刷新整个画面
	void PutDownStone(int x,int y);//下棋
public slots:
	void OnMouseReleaseOnBoardSense(QPoint *pos);//鼠标在棋盘上空释放
};
