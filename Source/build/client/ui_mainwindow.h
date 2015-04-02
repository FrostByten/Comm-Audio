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
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QGridLayout *Settings;
    QSlider *horizontalSlider;
    QPushButton *playButton;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *widget;
    QVBoxLayout *ChatLayout;
    QPlainTextEdit *plainTextEdit_2;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit;
    QPushButton *sendButton;
    QRadioButton *micRButton;
    QTabWidget *TabArea;
    QWidget *People;
    QTableView *tableView_2;
    QWidget *SongList;
    QWidget *PlayList;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1033, 659);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setWindowOpacity(1);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        Settings = new QGridLayout();
        Settings->setSpacing(6);
        Settings->setObjectName(QStringLiteral("Settings"));
        Settings->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        Settings->addWidget(horizontalSlider, 1, 1, 1, 1);

        playButton = new QPushButton(centralWidget);
        playButton->setObjectName(QStringLiteral("playButton"));

        Settings->addWidget(playButton, 1, 0, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        Settings->addWidget(pushButton, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        Settings->addWidget(pushButton_2, 2, 1, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));

        Settings->addWidget(widget, 0, 0, 1, 2);

        Settings->setRowStretch(0, 1);
        Settings->setColumnStretch(0, 1);
        Settings->setColumnStretch(1, 1);

        gridLayout->addLayout(Settings, 0, 2, 1, 1);

        ChatLayout = new QVBoxLayout();
        ChatLayout->setSpacing(6);
        ChatLayout->setObjectName(QStringLiteral("ChatLayout"));
        plainTextEdit_2 = new QPlainTextEdit(centralWidget);
        plainTextEdit_2->setObjectName(QStringLiteral("plainTextEdit_2"));

        ChatLayout->addWidget(plainTextEdit_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setMaximumSize(QSize(16777215, 24));

        horizontalLayout->addWidget(plainTextEdit);

        sendButton = new QPushButton(centralWidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));

        horizontalLayout->addWidget(sendButton);

        micRButton = new QRadioButton(centralWidget);
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


        ChatLayout->addLayout(horizontalLayout);

        ChatLayout->setStretch(0, 1);

        gridLayout->addLayout(ChatLayout, 0, 3, 1, 1);

        TabArea = new QTabWidget(centralWidget);
        TabArea->setObjectName(QStringLiteral("TabArea"));
        People = new QWidget();
        People->setObjectName(QStringLiteral("People"));
        tableView_2 = new QTableView(People);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));
        tableView_2->setGeometry(QRect(0, 0, 311, 571));
        TabArea->addTab(People, QString());
        SongList = new QWidget();
        SongList->setObjectName(QStringLiteral("SongList"));
        TabArea->addTab(SongList, QString());
        PlayList = new QWidget();
        PlayList->setObjectName(QStringLiteral("PlayList"));
        TabArea->addTab(PlayList, QString());

        gridLayout->addWidget(TabArea, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

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

        TabArea->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        playButton->setText(QApplication::translate("MainWindow", "Play", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Skip Forward", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Skip Back", 0));
        plainTextEdit_2->setPlainText(QApplication::translate("MainWindow", "Lewis: Linux is great hur dur dur", 0));
        sendButton->setText(QApplication::translate("MainWindow", "Send", 0));
#ifndef QT_NO_TOOLTIP
        micRButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        TabArea->setTabText(TabArea->indexOf(People), QApplication::translate("MainWindow", "People", 0));
        TabArea->setTabText(TabArea->indexOf(SongList), QApplication::translate("MainWindow", "Songs", 0));
        TabArea->setTabText(TabArea->indexOf(PlayList), QApplication::translate("MainWindow", "Play List", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
