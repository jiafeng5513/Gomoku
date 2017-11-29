#include "QtGomoku.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGomoku w;
	w.show();
	return a.exec();
}
