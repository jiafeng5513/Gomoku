#include "BoardScene.h"
#include <QGraphicsSceneMouseEvent>
#include <qDebug>

BoardScene::BoardScene(QObject *parent)
	: QGraphicsScene(parent)
{
}

BoardScene::~BoardScene()
{
}
/*
 * 重载鼠标移动事件
 */
void BoardScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	event->pos().x();
	//qDebug() << "X:"<<event->pos().x()<<"Y:"<< event->pos().y();
}
/*
 * 鼠标按下
 */
void BoardScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
}
/*
 * 鼠标释放
 */
void BoardScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	qDebug() << "X:" << event->scenePos().x() << "Y:" << event->scenePos().y()<<"RX:" << event->scenePos().rx() << "RY:" << event->scenePos().ry();
	//qDebug() << "X:" << event->screenPos().x() << "Y:" << event->screenPos().y()<<"RX:" << event->screenPos().rx() << "RY:" << event->screenPos().ry();
}
