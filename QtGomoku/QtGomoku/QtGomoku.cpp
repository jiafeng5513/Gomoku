#include "QtGomoku.h"
#include <QGraphicsItem>
#include <QPointF>

QtGomoku::QtGomoku(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_scene = new BoardScene();
	QPixmap*map = new QPixmap(":/QtGomoku/Resources/white.png");
	QGraphicsPixmapItem *qpi= new QGraphicsPixmapItem(*map);
	qpi->setPos(QPointF(30, 30));
	m_scene->addItem(qpi);
	ui.BoardView->setScene(m_scene);
}
