#pragma once

#include <QGraphicsScene>

class BoardScene : public QGraphicsScene
{
	Q_OBJECT

public:
	BoardScene(QObject *parent=NULL);
	~BoardScene();
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
