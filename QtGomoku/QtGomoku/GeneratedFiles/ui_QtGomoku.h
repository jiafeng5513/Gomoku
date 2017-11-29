/********************************************************************************
** Form generated from reading UI file 'QtGomoku.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGOMOKU_H
#define UI_QTGOMOKU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGomokuClass
{
public:
    QAction *action;
    QAction *actionAI;
    QAction *action_2;
    QAction *action_4;
    QAction *action_5;
    QWidget *centralWidget;
    QGraphicsView *BoardView;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QListView *listView_History;
    QGroupBox *groupBox_2;
    QLabel *label_Coordinate;
    QGroupBox *groupBox_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_BlackOwner;
    QLabel *label_WhiteOwner;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_3;
    QMenu *menu_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtGomokuClass)
    {
        if (QtGomokuClass->objectName().isEmpty())
            QtGomokuClass->setObjectName(QStringLiteral("QtGomokuClass"));
        QtGomokuClass->setEnabled(true);
        QtGomokuClass->resize(908, 678);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtGomokuClass->sizePolicy().hasHeightForWidth());
        QtGomokuClass->setSizePolicy(sizePolicy);
        QtGomokuClass->setStyleSheet(QStringLiteral("background-image: url(:/QtGomoku/Resources/bg.png);"));
        action = new QAction(QtGomokuClass);
        action->setObjectName(QStringLiteral("action"));
        actionAI = new QAction(QtGomokuClass);
        actionAI->setObjectName(QStringLiteral("actionAI"));
        actionAI->setCheckable(true);
        action_2 = new QAction(QtGomokuClass);
        action_2->setObjectName(QStringLiteral("action_2"));
        action_2->setCheckable(true);
        action_4 = new QAction(QtGomokuClass);
        action_4->setObjectName(QStringLiteral("action_4"));
        action_4->setCheckable(true);
        action_5 = new QAction(QtGomokuClass);
        action_5->setObjectName(QStringLiteral("action_5"));
        centralWidget = new QWidget(QtGomokuClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        BoardView = new QGraphicsView(centralWidget);
        BoardView->setObjectName(QStringLiteral("BoardView"));
        BoardView->setGeometry(QRect(10, 10, 600, 600));
        BoardView->setStyleSheet(QStringLiteral("background-image: url(:/QtGomoku/Resources/board.png);"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(620, 10, 271, 351));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listView_History = new QListView(groupBox);
        listView_History->setObjectName(QStringLiteral("listView_History"));

        gridLayout->addWidget(listView_History, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(620, 370, 271, 111));
        label_Coordinate = new QLabel(groupBox_2);
        label_Coordinate->setObjectName(QStringLiteral("label_Coordinate"));
        label_Coordinate->setGeometry(QRect(40, 40, 201, 31));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(30);
        label_Coordinate->setFont(font);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(620, 490, 271, 121));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 30, 40, 40));
        label->setStyleSheet(QStringLiteral("background-image: url(:/QtGomoku/Resources/black.png);"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(170, 30, 40, 40));
        label_2->setStyleSheet(QStringLiteral("background-image: url(:/QtGomoku/Resources/white.png);"));
        label_BlackOwner = new QLabel(groupBox_3);
        label_BlackOwner->setObjectName(QStringLiteral("label_BlackOwner"));
        label_BlackOwner->setGeometry(QRect(50, 80, 51, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221 Light"));
        font1.setPointSize(18);
        label_BlackOwner->setFont(font1);
        label_WhiteOwner = new QLabel(groupBox_3);
        label_WhiteOwner->setObjectName(QStringLiteral("label_WhiteOwner"));
        label_WhiteOwner->setGeometry(QRect(180, 80, 51, 31));
        label_WhiteOwner->setFont(font1);
        QtGomokuClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtGomokuClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 908, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_3 = new QMenu(menu);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        QtGomokuClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtGomokuClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtGomokuClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtGomokuClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtGomokuClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(actionAI);
        menu->addAction(action_2);
        menu->addAction(menu_3->menuAction());
        menu_3->addAction(action_4);
        menu_3->addAction(action_5);
        menu_2->addAction(action);

        retranslateUi(QtGomokuClass);

        QMetaObject::connectSlotsByName(QtGomokuClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtGomokuClass)
    {
        QtGomokuClass->setWindowTitle(QApplication::translate("QtGomokuClass", "QtGomoku", Q_NULLPTR));
        action->setText(QApplication::translate("QtGomokuClass", "\345\205\263\344\272\216", Q_NULLPTR));
        actionAI->setText(QApplication::translate("QtGomokuClass", "AI\346\211\247\351\273\221\345\205\210\350\241\214", Q_NULLPTR));
        action_2->setText(QApplication::translate("QtGomokuClass", "\347\216\251\345\256\266\346\211\247\351\273\221\345\205\210\350\241\214", Q_NULLPTR));
        action_4->setText(QApplication::translate("QtGomokuClass", "\350\277\236\347\217\240", Q_NULLPTR));
        action_5->setText(QApplication::translate("QtGomokuClass", "\344\270\211\345\244\247\347\246\201\346\211\213", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("QtGomokuClass", "\350\220\275\345\255\220\350\256\260\345\275\225", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("QtGomokuClass", "\345\235\220\346\240\207", Q_NULLPTR));
        label_Coordinate->setText(QApplication::translate("QtGomokuClass", "X:15   Y:15", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("QtGomokuClass", "\347\216\251\345\256\266\344\277\241\346\201\257", Q_NULLPTR));
        label->setText(QString());
        label_2->setText(QString());
        label_BlackOwner->setText(QApplication::translate("QtGomokuClass", "\347\216\251\345\256\266", Q_NULLPTR));
        label_WhiteOwner->setText(QApplication::translate("QtGomokuClass", "AI", Q_NULLPTR));
        menu->setTitle(QApplication::translate("QtGomokuClass", "\350\256\276\347\275\256", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("QtGomokuClass", "\350\247\204\345\210\231", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("QtGomokuClass", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtGomokuClass: public Ui_QtGomokuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGOMOKU_H
