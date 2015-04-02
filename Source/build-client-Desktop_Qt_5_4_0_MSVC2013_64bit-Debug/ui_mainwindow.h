/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab_6;
    QTableView *tableView_2;
    QWidget *tab_7;
    QWidget *tab_8;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_3;
    QSlider *horizontalSlider;
    QPushButton *playButton;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *plainTextEdit_2;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;
    QPushButton *sendButton;
    QRadioButton *micRButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1033, 659);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 1011, 581));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(gridLayoutWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        tableView_2 = new QTableView(tab_6);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setGeometry(QRect(0, 0, 241, 571));
        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        tabWidget->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        tabWidget->addTab(tab_8, QString());

        gridLayout->addWidget(tabWidget, 0, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        gridLayout->addLayout(verticalLayout, 0, 4, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalSlider = new QSlider(gridLayoutWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout_3->addWidget(horizontalSlider, 0, 1, 1, 1);

        playButton = new QPushButton(gridLayoutWidget);
        playButton->setObjectName(QStringLiteral("playButton"));

        gridLayout_3->addWidget(playButton, 0, 0, 1, 1);

        pushButton = new QPushButton(gridLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_3->addWidget(pushButton, 1, 0, 1, 1);

        pushButton_2 = new QPushButton(gridLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_3->addWidget(pushButton_2, 1, 1, 1, 1);


        gridLayout->addLayout(gridLayout_3, 0, 2, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        plainTextEdit_2 = new QPlainTextEdit(gridLayoutWidget);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));

        verticalLayout_2->addWidget(plainTextEdit_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        plainTextEdit = new QPlainTextEdit(gridLayoutWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        horizontalLayout->addWidget(plainTextEdit);

        sendButton = new QPushButton(gridLayoutWidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));

        horizontalLayout->addWidget(sendButton);

        micRButton = new QRadioButton(gridLayoutWidget);
        micRButton->setObjectName(QStringLiteral("micRButton"));
        micRButton->setEnabled(true);
        micRButton->setStyleSheet(QLatin1String("QRadioButton::indicator  {\n"
"    width: 32px;\n"
"    height: 32px;\n"
"}\n"
" \n"
"QRadioButton::indicator:unchecked  {\n"
"    image: url(:/images/mic-stopped.png);\n"
"}\n"
" \n"
"QRadioButton::indicator:checked  {\n"
"    image: url(:/images/mic-running.png);\n"
"}"));
        micRButton->setChecked(false);

        horizontalLayout->addWidget(micRButton);


        verticalLayout_2->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout_2, 0, 3, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1033, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("MainWindow", "People", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("MainWindow", "Songs", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QApplication::translate("MainWindow", "Play List", 0));
        playButton->setText(QApplication::translate("MainWindow", "Play", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Skip Forward", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Skip Back", 0));
        plainTextEdit_2->setPlainText(QApplication::translate("MainWindow", "Lewis: Linux is great hur dur dur", 0));
        sendButton->setText(QApplication::translate("MainWindow", "Send", 0));
#ifndef QT_NO_TOOLTIP
        micRButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
