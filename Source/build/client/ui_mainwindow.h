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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *ChatLayout;
    QListView *chat;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *sendButton;
    QRadioButton *micRButton;
    QTabWidget *TabArea;
    QWidget *People;
    QGridLayout *gridLayout_3;
    QListWidget *list_people;
    QWidget *SongList;
    QGridLayout *gridLayout_4;
    QListWidget *list_songs;
    QWidget *PlayList;
    QVBoxLayout *verticalLayout_2;
    QListView *list_playlist;
    QGridLayout *Settings;
    QPushButton *pushButton;
    QWidget *widget;
    QGridLayout *gridLayout_5;
    QPushButton *pause_Btn;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QSlider *horizontalSlider;
    QPushButton *mute_Btn;
    QPushButton *skip_back_Btn;
    QPushButton *play_Btn;
    QPushButton *skip_for_Btn;
    QMenuBar *menuBar;
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
        ChatLayout = new QVBoxLayout();
        ChatLayout->setSpacing(6);
        ChatLayout->setObjectName(QStringLiteral("ChatLayout"));
        chat = new QListView(centralWidget);
        chat->setObjectName(QStringLiteral("chat"));

        ChatLayout->addWidget(chat);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

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
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(People->sizePolicy().hasHeightForWidth());
        People->setSizePolicy(sizePolicy2);
        People->setStyleSheet(QStringLiteral(""));
        gridLayout_3 = new QGridLayout(People);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        list_people = new QListWidget(People);
        list_people->setObjectName(QStringLiteral("list_people"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(list_people->sizePolicy().hasHeightForWidth());
        list_people->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(list_people, 0, 0, 1, 1);

        TabArea->addTab(People, QString());
        SongList = new QWidget();
        SongList->setObjectName(QStringLiteral("SongList"));
        gridLayout_4 = new QGridLayout(SongList);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(2, 2, 2, 2);
        list_songs = new QListWidget(SongList);
        list_songs->setObjectName(QStringLiteral("list_songs"));

        gridLayout_4->addWidget(list_songs, 0, 0, 1, 1);

        TabArea->addTab(SongList, QString());
        PlayList = new QWidget();
        PlayList->setObjectName(QStringLiteral("PlayList"));
        verticalLayout_2 = new QVBoxLayout(PlayList);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        list_playlist = new QListView(PlayList);
        list_playlist->setObjectName(QStringLiteral("list_playlist"));

        verticalLayout_2->addWidget(list_playlist);

        TabArea->addTab(PlayList, QString());

        gridLayout->addWidget(TabArea, 0, 1, 1, 1);

        Settings = new QGridLayout();
        Settings->setSpacing(6);
        Settings->setObjectName(QStringLiteral("Settings"));
        Settings->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        Settings->addWidget(pushButton, 2, 1, 1, 1);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_5 = new QGridLayout(widget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        TabArea->raise();

        Settings->addWidget(widget, 0, 0, 1, 4);

        pause_Btn = new QPushButton(centralWidget);
        pause_Btn->setObjectName(QStringLiteral("pause_Btn"));

        Settings->addWidget(pause_Btn, 1, 1, 1, 1);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        Settings->addWidget(pushButton_3, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        Settings->addWidget(pushButton_2, 2, 3, 1, 1);

        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        Settings->addWidget(horizontalSlider, 4, 1, 1, 1);

        mute_Btn = new QPushButton(centralWidget);
        mute_Btn->setObjectName(QStringLiteral("mute_Btn"));

        Settings->addWidget(mute_Btn, 1, 3, 1, 1);

        skip_back_Btn = new QPushButton(centralWidget);
        skip_back_Btn->setObjectName(QStringLiteral("skip_back_Btn"));

        Settings->addWidget(skip_back_Btn, 4, 0, 1, 1);

        play_Btn = new QPushButton(centralWidget);
        play_Btn->setObjectName(QStringLiteral("play_Btn"));

        Settings->addWidget(play_Btn, 1, 0, 1, 1);

        skip_for_Btn = new QPushButton(centralWidget);
        skip_for_Btn->setObjectName(QStringLiteral("skip_for_Btn"));

        Settings->addWidget(skip_for_Btn, 4, 3, 1, 1);

        Settings->setRowStretch(0, 1);
        Settings->setColumnStretch(0, 1);

        gridLayout->addLayout(Settings, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1033, 18));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        TabArea->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        sendButton->setText(QApplication::translate("MainWindow", "Send", 0));
#ifndef QT_NO_TOOLTIP
        micRButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        TabArea->setTabText(TabArea->indexOf(People), QApplication::translate("MainWindow", "People", 0));
        TabArea->setTabText(TabArea->indexOf(SongList), QApplication::translate("MainWindow", "Songs", 0));
        TabArea->setTabText(TabArea->indexOf(PlayList), QApplication::translate("MainWindow", "Play List", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Request File", 0));
        pause_Btn->setText(QApplication::translate("MainWindow", "Pause", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Settings", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Request URL", 0));
        mute_Btn->setText(QApplication::translate("MainWindow", "Mute User", 0));
        skip_back_Btn->setText(QApplication::translate("MainWindow", "Skip Back", 0));
        play_Btn->setText(QApplication::translate("MainWindow", "Play", 0));
        skip_for_Btn->setText(QApplication::translate("MainWindow", "Skip Forward", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
