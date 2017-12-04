#include "QtGomoku.h"
#include <QtWidgets/QApplication>
#include <qDebug>
#include <QFile>
#include <QDateTime>
#include <QMutex>
/*
 * 实现日志功能
 * 
 * 注释掉的部分能记录更多的信息单数更耗时
 */
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	// 加锁
	static QMutex mutex;
	mutex.lock();

	//QByteArray localMsg = msg.toLocal8Bit();

	//QString strMsg("");
	//switch (type)
	//{
	//case QtDebugMsg:
	//	strMsg = QString("Debug:");
	//	break;
	//case QtWarningMsg:
	//	strMsg = QString("Warning:");
	//	break;
	//case QtCriticalMsg:
	//	strMsg = QString("Critical:");
	//	break;
	//case QtFatalMsg:
	//	strMsg = QString("Fatal:");
	//	break;
	//}

	//// 设置输出信息格式
	//QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	//QString strMessage = QString("Message:%1 File:%2  Line:%3  Function:%4  DateTime:%5")
	//	.arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

	//// 输出信息至文件中（读写、追加形式）
	//QFile file("Gomokulog.txt");
	//file.open(QIODevice::ReadWrite | QIODevice::Append);
	//QTextStream stream(&file);
	//stream << strMessage << "\r\n";
	//file.flush();
	//file.close();


	QString txtMessage;

	switch (type)
	{
	case QtDebugMsg:    //调试信息提示
		txtMessage = QString("Debug: %1").arg(msg);
		break;

	case QtWarningMsg:    //一般的warning提示
		txtMessage = QString("Warning: %1").arg(msg);
		break;

	case QtCriticalMsg:    //严重错误提示
		txtMessage = QString("Critical: %1").arg(msg);
		break;

	case QtFatalMsg:    //致命错误提示
		txtMessage = QString("Fatal: %1").arg(msg);
		abort();
	}

	//保存输出相关信息到指定文件
	QFile outputFile("Gomokulog.txt");
	outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream textStream(&outputFile);
	textStream << txtMessage << endl;
	// 解锁
	mutex.unlock();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qInstallMessageHandler(customMessageHandler);
	QtGomoku w;
	w.show();
	return a.exec();
}
