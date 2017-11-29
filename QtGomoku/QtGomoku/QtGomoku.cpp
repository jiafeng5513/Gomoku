#include "QtGomoku.h"
#include <QGraphicsItem>
#include <QPointF>
#include <csignal>
#include <qDebug>
#include "Alpha-Beta.h"

/*
 * 构造和初始化
 */
QtGomoku::QtGomoku(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_scene = new BoardScene(ui.BoardView);
	connect(m_scene, SIGNAL(BoardSenseMouseRelease(QPoint*)), this, SLOT(OnMouseReleaseOnBoardSense(QPoint*)));
	m_scene->setSceneRect(0, 0
		, static_cast<QWidget *>(m_scene->parent())->size().width()
		, static_cast<QWidget *>(m_scene->parent())->size().height());
	ui.BoardView->setScene(m_scene);

	WhiteStone = new QPixmap(":/QtGomoku/Resources/white.png");
	BlackStone = new QPixmap(":/QtGomoku/Resources/black.png");
	for(int i=0;i<GRID_NUM;i++)
	{
		for (int j=0;j<GRID_NUM;j++)
		{
			BoardMap[i][j] = space;
		}
	}
	m_ai = new AiAgent();
}
/*
 * 鼠标在棋盘上空释放
 */
void QtGomoku::OnMouseReleaseOnBoardSense(QPoint* pos)
{
	int qx = pos->x() / 40;
	int qy = pos->y() / 40;

	PutDownStone(qx, qy);
	//函数应该在这里结束,AI的运行用额外的线程进行
	POINT temp=m_ai->GetAiAction(qx, qy);
	PutDownStone(temp.x, temp.y);
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
	if(BoardMap[x][y]==space)//这个位置是空的
	{
		QGraphicsPixmapItem *qpi;
		if(CurrentTurn == Human)//人类玩家下棋
		{
			BoardMap[x][y] = HumanColor;
			qpi = new QGraphicsPixmapItem((HumanColor == black ? *BlackStone : *WhiteStone));
			CurrentTurn = Ai;
			QString temp = QStringLiteral("玩家:  x:");temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
		}
		else
		{
			BoardMap[x][y] = AiColor;
			qpi = new QGraphicsPixmapItem((AiColor == black ? *BlackStone : *WhiteStone));
			CurrentTurn = Human;
			QString temp = QStringLiteral("AI:  x:"); temp.append(QString::fromStdString(std::to_string(x))); temp.append(",y:"); temp.append(QString::fromStdString(std::to_string(y)));
			ui.listWidget_History->addItem(temp);
		}
		qpi->setPos(QPoint(x * 40, y * 40));
		m_scene->addItem(qpi);
	}
}
