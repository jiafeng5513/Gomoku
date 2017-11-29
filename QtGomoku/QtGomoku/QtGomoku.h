#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGomoku.h"
#include "BoardScene.h"

class QtGomoku : public QMainWindow
{
	Q_OBJECT

public:
	QtGomoku(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGomokuClass ui;
	BoardScene * m_scene;
};
