#include "QtGomoku.h"
#include <QGraphicsItem>
#include <QPointF>
#include <csignal>
#include <qDebug>
#include <QMessageBox>
#include <QFile>
#include <QString>


/*
 * 构造和初始化
 */
QtGomoku::QtGomoku(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setFixedSize(this->width(), this->height());  //禁止调整大小
	m_scene = new BoardScene(ui.BoardView);
	connect(m_scene, SIGNAL(BoardSenseMouseRelease(QPoint*)), this, SLOT(OnMouseReleaseOnBoardSense(QPoint*)));
	m_scene->setSceneRect(0, 0
		, static_cast<QWidget *>(m_scene->parent())->size().width()
		, static_cast<QWidget *>(m_scene->parent())->size().height());
	ui.BoardView->setScene(m_scene);

	WhiteStone = new QPixmap(":/QtGomoku/Resources/white.png");
	BlackStone = new QPixmap(":/QtGomoku/Resources/black.png");
	MarkForLastStep = new QPixmap(":/QtGomoku/Resources/kh.png");
	Mark= new QGraphicsPixmapItem(*MarkForLastStep);
	for(int i=0;i<GRID_NUM;i++)
	{
		for (int j=0;j<GRID_NUM;j++)
		{
			BoardMap[i][j] = space;
		}
	}
	robot = new AlphaBetaRobot();
	connect(this, SIGNAL(IsTimeForAiToCalculate(POINT*)), robot, SLOT(getAiResponse(POINT*)), Qt::QueuedConnection);
	connect(robot, SIGNAL(AIComplete(POINT*)), this, SLOT(OnAiComplete(POINT*)), Qt::QueuedConnection);
	Count = 1;
	ui.radioButton_PVE->setChecked(true);
	ui.radioButton_HumanBlack->setChecked(true);
	ui.radioButton_UnableForbidden->setChecked(true);

	ui.radioButton_PVP->setChecked(false);
	ui.radioButton_HumanWhite->setChecked(false);
	ui.radioButton_EnableForbidden->setChecked(false);
}
/*
 * 鼠标在棋盘上空释放(玩家下棋)
 */
void QtGomoku::OnMouseReleaseOnBoardSense(QPoint* pos)
{
	if (m_isGameOver==true)//游戏已经结束,不能继续落子
	{
		return;
	}
	if (isPVE == true)//目前是人机对战模式
	{
		if (CurrentTurn == Human)
		{
			int qx = pos->x() / 40;
			int qy = pos->y() / 40;

			PutDownStone(qx, qy);//人类完成落子
			POINT * _humanPos = new  POINT;
			_humanPos->x = qx;
			_humanPos->y = qy;
			if (isGameOver() == true)//游戏不能继续,不通知AI,禁用该函数禁止玩家继续落子
			{
				//禁用该函数
				m_isGameOver = true;
				//玩家胜利
				QMessageBox::information(this, QStringLiteral("恭喜"), QStringLiteral("您打败了AI!"));
			}
			else//如果游戏还能继续,则通知AI进行计算
			{
				emit IsTimeForAiToCalculate(_humanPos);
				ui.statusBar->showMessage(QStringLiteral("AI正在计算..."));
			}
			//qDebug() << "emit IsTimeForAiToCalculate, human pos:("<< qx << ","<<qy<<")";
		}
	}
	else//目前是人人对战模式
	{
		int qx = pos->x() / 40;
		int qy = pos->y() / 40;

		PutDownStone(qx, qy);//交替落子
		if (isGameOver() == true)//游戏不能继续,不通知AI,禁用该函数禁止玩家继续落子
		{
			m_isGameOver = true;
			QMessageBox::information(this, QStringLiteral("游戏结束"), QStringLiteral("请自行判断输赢>_<"));
		}
	}
	
}
/*
 * 根据当前棋局数组刷新整个画面
 */
void QtGomoku::UpdateWholeBoardView()
{
	m_scene->clear();
	for (int i=0;i<GRID_NUM;i++)
	{
		for(int j=0;j<GRID_NUM;j++)
		{
			QGraphicsPixmapItem* Black = new QGraphicsPixmapItem(*BlackStone);
			QGraphicsPixmapItem* White = new QGraphicsPixmapItem(*WhiteStone);
			switch(BoardMap[i][j])
			{
			case white:
				White->setPos(QPoint(i * 40, j * 40));
				m_scene->addItem(White);
				break;
			case black:
				Black->setPos(QPoint(i * 40, j * 40));
				m_scene->addItem(Black);
				break;
			case space:
				break;
			default:
				break;
			}
		}
	}
}
/*
 * 在(x,y)点(x和y∈[0,14])落下指定一方的棋子,并交换棋权
 */
void QtGomoku::PutDownStone(int x, int y)
{
	if(x<0||y<0||x>14||y>>14)
	{
		QMessageBox::information(this, QStringLiteral("错误!"), QStringLiteral("落子坐标非法!"));
		return;
	}
	if (BoardMap[x][y]!=space)
	{
		QMessageBox::information(this, QStringLiteral("错误!"), QStringLiteral("落子坐标已经有其他棋子!"));
		return;
	}
	//这个位置是空的
	m_scene->removeItem(Mark);
	QGraphicsPixmapItem *qpi;
	QString temp = QString::number(Count, 10);  
	/*QGraphicsTextItem **/txtitem = new QGraphicsTextItem(temp);
	
	txtitem->setFont(QFont("Microsoft YaHei", 12, QFont::Normal));
	if (isPVE == true)//人机模式
	{
		if (CurrentTurn == Human)//人类玩家下棋
		{
			BoardMap[x][y] = HumanColor;
			qpi = new QGraphicsPixmapItem((HumanColor == black ? *BlackStone : *WhiteStone));
			CurrentTurn = Ai;
			QString temp = QStringLiteral("玩家:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
			txtitem->setDefaultTextColor((HumanColor == black ? QColor(255, 255, 255) : QColor(0, 0, 0)));
		}
		else
		{
			BoardMap[x][y] = AiColor;
			qpi = new QGraphicsPixmapItem((AiColor == black ? *BlackStone : *WhiteStone));
			CurrentTurn = Human;
			QString temp = QStringLiteral("AI:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
			txtitem->setDefaultTextColor((HumanColor == black ? QColor(0, 0, 0) : QColor(255, 255, 255)));
		}
	}
	else//人人模式
	{
		if (CurrentColor == black)//黑棋
		{
			BoardMap[x][y] = black;
			qpi = new QGraphicsPixmapItem( *BlackStone );
			CurrentColor = white;
			QString temp = QStringLiteral("黑棋:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
			txtitem->setDefaultTextColor(QColor(255, 255, 255));
		}
		else//白棋
		{
			BoardMap[x][y] = white;
			qpi = new QGraphicsPixmapItem( *WhiteStone);
			CurrentColor = black;
			QString temp = QStringLiteral("白棋:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
			txtitem->setDefaultTextColor( QColor(0, 0, 0));
		}
	}
	
	//放棋子
	ui.listWidget_History->scrollToBottom();
	qpi->setPos(QPoint(x * 40, y * 40));
	m_scene->addItem(qpi);
	//写字
	int xaddin = (Count <= 9 && Count >= 0 ? 11 : (Count >= 10 && Count <= 99 ? 7.2 : 3.4));
	txtitem->setPos(QPoint(x * 40+ xaddin, y * 40+5.5));//设置要放置的的位置//x轴偏置量需要按照位数计算
	m_scene->addItem(txtitem);//添加item到scene上
	//上一步标记
	Mark->setPos(QPoint(x * 40, y * 40));
	m_scene->addItem(Mark);
	Count++;
}
/*
 * 判断游戏是否结束(存在bug,某些情况下会误判)
 */
bool QtGomoku::isGameOver()
{
	/*
	 * 全局搜索,跳过没有棋子的点
	 */
	//遍历这个棋子横竖撇捺四个方向,发现连五判赢
	int vCount = 0, hCount = 0, lCount = 0, rCount = 0;
	for (int x=0;x<GRID_NUM;x++)
	{
		for (int y = 0; y < GRID_NUM; y++)
		{
			if(BoardMap[x][y]==space)
			{
				continue;
			}
			Color _color = BoardMap[x][y];//当前落子的颜色
			for (int i = 0; i < GRID_NUM; i++)//水平和竖直
			{
				hCount = (BoardMap[i][y] == _color ? hCount + 1 : 0);
				vCount = (BoardMap[x][i] == _color ? vCount + 1 : 0);
				if (vCount >= 5 || hCount >= 5)
				{
					return true;//游戏已经结束
				}

			}
			int m = GRID_NUM - abs(x - y);//主对角线方向元素数量
			int n = GRID_NUM - abs(GRID_NUM - (x + y + 1));//次对角线方向元素数量
			for (int i = -std::min(x, y); i < m - std::min(x, y); i++)
			{
				rCount = (BoardMap[x + i][y + i] == _color ? rCount + 1 : 0);//主对角线方向
				if (rCount >= 5)
				{
					return true;//游戏已经结束
				}
			}
			int imax = n + (x - y) - 1 - std::min(x, GRID_NUM - 1 - y);
			int jmax = n - imax;
			for (int i = -imax; i < jmax; i++)
			{
				lCount = (BoardMap[x - i][y + i] == _color ? lCount + 1 : 0);//次对角线方向
				if (lCount >= 5)
				{
					return true;//游戏已经结束
				}
			}
		}
	}
	return false;//游戏还没有结束
}
/*
 * 响应AI计算完毕
 */
void QtGomoku::OnAiComplete(POINT * pos)
{
	//qDebug() << "OnAiComplete is running...";
	PutDownStone(pos->x, pos->y);
	if (isGameOver() == true)//游戏不能继续,禁止玩家落子
	{
		//游戏终止
		m_isGameOver = true;
		//AI胜利
		QMessageBox::information(this, QStringLiteral("很遗憾"), QStringLiteral("AI已经取胜!"));
	}		
	ui.statusBar->clearMessage();
	ui.statusBar->showMessage(QStringLiteral("AI计算完毕!"));
}
/*
 * 按键:新游戏
 */
void QtGomoku::OnNewGame()
{
	//禁用所有调整:游戏过程中不能调整参数
	ui.radioButton_PVE->setEnabled(false);
	ui.radioButton_PVP->setEnabled(false);
	ui.radioButton_EnableForbidden->setEnabled(false);
	ui.radioButton_UnableForbidden->setEnabled(false);
	ui.radioButton_HumanBlack->setEnabled(false);
	ui.radioButton_HumanWhite->setEnabled(false);
	ui.spinBox->setEnabled(false);
	//初始化
	//清空棋盘数组
	for (int i = 0; i<GRID_NUM; i++)
	{
		for (int j = 0; j<GRID_NUM; j++)
		{
			BoardMap[i][j] = space;
		}
	}
	//清空棋盘
	m_scene = new BoardScene(ui.BoardView);
	connect(m_scene, SIGNAL(BoardSenseMouseRelease(QPoint*)), this, SLOT(OnMouseReleaseOnBoardSense(QPoint*)));
	m_scene->setSceneRect(0, 0
		, static_cast<QWidget *>(m_scene->parent())->size().width()
		, static_cast<QWidget *>(m_scene->parent())->size().height());
	ui.BoardView->setScene(m_scene);
	//AI初始化
	robot = new AlphaBetaRobot();
	//从第一步开始
	Count = 1;
	ui.listWidget_History->clear();
	//
	CurrentTurn = (isHumanGetBlack == true ? Human : Ai);
	CurrentColor = black;
	m_isGameOver = false;
	POINT * _SignalforAiFirst = new POINT();
	if (isPVE==true&&isHumanGetBlack==false)//人机对战且人类玩家选择执白后手
	{
		_SignalforAiFirst->x = -1;
		_SignalforAiFirst->y = -1;
		emit IsTimeForAiToCalculate(_SignalforAiFirst);
	}
}
/*
 * 按键:悔棋
 */
void QtGomoku::OnUndo()
{
	//人机模式下,只有轮到玩家下棋而玩家尚未落子的时候才能悔棋
	//1.通知AI撤销上一步自己的落子和上一步玩家的落子
	//2.棋盘数组中消除上一步的落子
	//3.在putdown函数中维护一个序列数组,大小为225,记录双方的落子顺序.
	//4.上述工作结束后,调用刷新函数,根据序列数组将双方的棋子按顺序画上
	//5.通知AI开始计算,通知参数为人类玩家最后一次落子的位置
	//人人模式下,随时都可以悔棋
	//1.若白棋刚落子,按悔棋,此时视为黑方按的悔棋,撤销白棋的上一手和黑棋的上一手
	//2.若黑棋刚落子,按悔棋,此时视为白方按的悔棋,撤销黑棋的上一手和白棋的上一手
	//3.然后调用刷新函数按照序列数组刷新
	//可以加一个功能把序列数组存成文件,然后加入复盘功能,便于记录一些信息以供调试
}
/*
 * 按键:结束游戏
 */
void QtGomoku::OnStopGame()
{
	m_isGameOver = true;
	//禁用所有调整:游戏过程中不能调整参数
	ui.radioButton_PVE->setEnabled(true);
	ui.radioButton_PVP->setEnabled(true);
	ui.radioButton_EnableForbidden->setEnabled(true);
	ui.radioButton_UnableForbidden->setEnabled(true);
	ui.radioButton_HumanBlack->setEnabled(true);
	ui.radioButton_HumanWhite->setEnabled(true);
	ui.spinBox->setEnabled(true);
}
/*
 * 按键:帮助
 */
void QtGomoku::OnHelp()
{
}
/*
 * 单选:玩家执黑
 */
void QtGomoku::OnCheckedHumanBlack()
{
	if(ui.radioButton_HumanBlack->isChecked()==false)
	{
		ui.radioButton_HumanBlack->setChecked(false);
		ui.radioButton_HumanWhite->setChecked(true);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("电脑") : QStringLiteral("玩家1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("玩家") : QStringLiteral("玩家2")));
		isHumanGetBlack = false;
		HumanColor = white;
		AiColor = black;
	}
	else
	{
		ui.radioButton_HumanBlack->setChecked(true);
		ui.radioButton_HumanWhite->setChecked(false);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("玩家") : QStringLiteral("玩家1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("电脑") : QStringLiteral("玩家2")));
		isHumanGetBlack = true;
		HumanColor = black;
		AiColor = white;
	}
}
/*
 * 单选:玩家执白
 */
void QtGomoku::OnCheckedHumanWhite()
{
	if (ui.radioButton_HumanWhite->isChecked()==false)
	{
		ui.radioButton_HumanBlack->setChecked(true);
		ui.radioButton_HumanWhite->setChecked(false);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("玩家") : QStringLiteral("玩家1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("电脑") : QStringLiteral("玩家2")));
		isHumanGetBlack = true;
		HumanColor = black;
		AiColor = white;
	}
	else
	{
		ui.radioButton_HumanBlack->setChecked(false);
		ui.radioButton_HumanWhite->setChecked(true);
		ui.label_BlackOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("电脑") : QStringLiteral("玩家1")));
		ui.label_WhiteOwner->setText((ui.radioButton_PVE->isChecked() ? QStringLiteral("玩家") : QStringLiteral("玩家2")));
		isHumanGetBlack = false;
		HumanColor = white;
		AiColor = black;
	}
}
/*
 * 单选:启动禁手
 */
void QtGomoku::OnCheckedEnableForbidden()
{
	if (ui.radioButton_EnableForbidden->isChecked()==false)
	{
		ui.radioButton_EnableForbidden->setChecked(false);
		ui.radioButton_UnableForbidden->setChecked(true);
	}
	else
	{
		ui.radioButton_EnableForbidden->setChecked(true);
		ui.radioButton_UnableForbidden->setChecked(false);
	}
}
/*
 * 单选:关闭禁手
 */
void QtGomoku::OnCheckedUnableForbidden()
{
	if (ui.radioButton_UnableForbidden->isChecked()==false)
	{
		ui.radioButton_EnableForbidden->setChecked(true);
		ui.radioButton_UnableForbidden->setChecked(false);
	}
	else
	{
		ui.radioButton_EnableForbidden->setChecked(false);
		ui.radioButton_UnableForbidden->setChecked(true);
	}
}
/*
 * 单选:人机对战
 */
void QtGomoku::OnCheckedPVE()
{
	if (ui.radioButton_PVE->isChecked()== false)
	{
		ui.radioButton_PVE->setChecked(false);//互斥选项
		ui.radioButton_PVP->setChecked(true);
		isPVE = false;
		ui.label_BlackOwner->setText(QStringLiteral("玩家1"));
		ui.label_WhiteOwner->setText(QStringLiteral("玩家2"));
		ui.radioButton_HumanBlack->setEnabled(false);
		ui.radioButton_HumanWhite->setEnabled(false);
	}else
	{
		ui.radioButton_PVE->setChecked(true);//互斥选项
		ui.radioButton_PVP->setChecked(false);
		isPVE = true;
		ui.label_BlackOwner->setText((ui.radioButton_HumanBlack->isChecked()? QStringLiteral("玩家"): QStringLiteral("电脑")));
		ui.label_WhiteOwner->setText((ui.radioButton_HumanWhite->isChecked()? QStringLiteral("玩家") : QStringLiteral("电脑")));
		ui.radioButton_HumanBlack->setEnabled(true);
		ui.radioButton_HumanWhite->setEnabled(true);
	}
	
}
/*
 * 单选:人人对战
 */
void QtGomoku::OnCheckedPVP()
{
	if (ui.radioButton_PVP->isChecked() == false)
	{
		ui.radioButton_PVP->setChecked(false);
		ui.radioButton_PVE->setChecked(true);//互斥选项
		
		isPVE = true;
		ui.label_BlackOwner->setText((ui.radioButton_HumanBlack->isChecked() ? QStringLiteral("玩家") : QStringLiteral("电脑")));
		ui.label_WhiteOwner->setText((ui.radioButton_HumanWhite->isChecked() ? QStringLiteral("玩家") : QStringLiteral("电脑")));
		ui.radioButton_HumanBlack->setEnabled(true);
		ui.radioButton_HumanWhite->setEnabled(true);
	}
	else
	{
		ui.radioButton_PVP->setChecked(true);
		ui.radioButton_PVE->setChecked(false);//互斥选项
		isPVE = false;
		ui.label_BlackOwner->setText(QStringLiteral("玩家1"));
		ui.label_WhiteOwner->setText(QStringLiteral("玩家2"));
		ui.radioButton_HumanBlack->setEnabled(false);
		ui.radioButton_HumanWhite->setEnabled(false);
	}
}
/*
 * Ai等级变动
 */
void QtGomoku::OnAiLevelChanged(int newValue)
{
	switch (newValue)
	{
	case 1:
		robot->LIMIT_DEPTH = 4;
		break;
	case 2:
		robot->LIMIT_DEPTH = 8;
		break;
	case 3:
		robot->LIMIT_DEPTH = 16;
		break;
	case 4:
		robot->LIMIT_DEPTH = 32;
		break;
	case 5:
		robot->LIMIT_DEPTH = 64;
		break;
	default:
		robot->LIMIT_DEPTH = 32;
		ui.spinBox->setValue(4);
		break;
	}
}
