/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Apr 8 19:33:59 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

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
    QGridLayout *Settings;
    QPushButton *select_URL_Btn;
    QPushButton *connect_Btn;
    QPushButton *play_Btn;
    QPushButton *download_Btn;
    QPushButton *pause_Btn;
    QSlider *volume;
    QPushButton *mute_Btn;
    QPushButton *select_file_Btn;
    QPushButton *skip_for_Btn;
    QSlider *seek_Bar;
    QGridLayout *BarGraph;
    QTabWidget *TabArea;
    QWidget *People;
    QGridLayout *gridLayout_3;
    QListView *list_people;
    QWidget *SongList;
    QGridLayout *gridLayout_4;
    QListView *list_songs;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(814, 435);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setWindowOpacity(1);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        ChatLayout = new QVBoxLayout();
        ChatLayout->setSpacing(6);
        ChatLayout->setObjectName(QString::fromUtf8("ChatLayout"));
        chat = new QListView(centralWidget);
        chat->setObjectName(QString::fromUtf8("chat"));

        ChatLayout->addWidget(chat);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        sendButton = new QPushButton(centralWidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        horizontalLayout->addWidget(sendButton);

        micRButton = new QRadioButton(centralWidget);
        micRButton->setObjectName(QString::fromUtf8("micRButton"));
        micRButton->setEnabled(true);
        micRButton->setStyleSheet(QString::fromUtf8("QRadioButton::indicator  {\n"
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

        Settings = new QGridLayout();
        Settings->setSpacing(6);
        Settings->setObjectName(QString::fromUtf8("Settings"));
        Settings->setSizeConstraint(QLayout::SetDefaultConstraint);
        select_URL_Btn = new QPushButton(centralWidget);
        select_URL_Btn->setObjectName(QString::fromUtf8("select_URL_Btn"));

        Settings->addWidget(select_URL_Btn, 3, 2, 1, 1);

        connect_Btn = new QPushButton(centralWidget);
        connect_Btn->setObjectName(QString::fromUtf8("connect_Btn"));

        Settings->addWidget(connect_Btn, 3, 0, 1, 1);

        play_Btn = new QPushButton(centralWidget);
        play_Btn->setObjectName(QString::fromUtf8("play_Btn"));

        Settings->addWidget(play_Btn, 3, 1, 1, 1);

        download_Btn = new QPushButton(centralWidget);
        download_Btn->setObjectName(QString::fromUtf8("download_Btn"));

        Settings->addWidget(download_Btn, 6, 2, 1, 1);

        pause_Btn = new QPushButton(centralWidget);
        pause_Btn->setObjectName(QString::fromUtf8("pause_Btn"));

        Settings->addWidget(pause_Btn, 2, 1, 1, 1);

        volume = new QSlider(centralWidget);
        volume->setObjectName(QString::fromUtf8("volume"));
        volume->setCursor(QCursor(Qt::SplitHCursor));
        volume->setValue(99);
        volume->setOrientation(Qt::Horizontal);

        Settings->addWidget(volume, 6, 1, 1, 1);

        mute_Btn = new QPushButton(centralWidget);
        mute_Btn->setObjectName(QString::fromUtf8("mute_Btn"));

        Settings->addWidget(mute_Btn, 2, 0, 1, 1);

        select_file_Btn = new QPushButton(centralWidget);
        select_file_Btn->setObjectName(QString::fromUtf8("select_file_Btn"));

        Settings->addWidget(select_file_Btn, 2, 2, 1, 1);

        skip_for_Btn = new QPushButton(centralWidget);
        skip_for_Btn->setObjectName(QString::fromUtf8("skip_for_Btn"));

        Settings->addWidget(skip_for_Btn, 6, 0, 1, 1);

        seek_Bar = new QSlider(centralWidget);
        seek_Bar->setObjectName(QString::fromUtf8("seek_Bar"));
        seek_Bar->setMaximum(999);
        seek_Bar->setOrientation(Qt::Horizontal);

        Settings->addWidget(seek_Bar, 1, 0, 1, 3);

        BarGraph = new QGridLayout();
        BarGraph->setSpacing(6);
        BarGraph->setObjectName(QString::fromUtf8("BarGraph"));

        Settings->addLayout(BarGraph, 0, 0, 1, 3);

        Settings->setRowStretch(0, 1);

        gridLayout->addLayout(Settings, 0, 2, 1, 1);

        TabArea = new QTabWidget(centralWidget);
        TabArea->setObjectName(QString::fromUtf8("TabArea"));
        People = new QWidget();
        People->setObjectName(QString::fromUtf8("People"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(People->sizePolicy().hasHeightForWidth());
        People->setSizePolicy(sizePolicy2);
        People->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(People);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        list_people = new QListView(People);
        list_people->setObjectName(QString::fromUtf8("list_people"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(list_people->sizePolicy().hasHeightForWidth());
        list_people->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(list_people, 0, 0, 1, 1);

        TabArea->addTab(People, QString());
        SongList = new QWidget();
        SongList->setObjectName(QString::fromUtf8("SongList"));
        gridLayout_4 = new QGridLayout(SongList);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(2, 2, 2, 2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        list_songs = new QListView(SongList);
        list_songs->setObjectName(QString::fromUtf8("list_songs"));

        gridLayout_4->addWidget(list_songs, 0, 0, 1, 1);

        progressBar = new QProgressBar(SongList);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setAutoFillBackground(false);
        progressBar->setValue(0);
        progressBar->setTextVisible(true);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setInvertedAppearance(false);

        gridLayout_4->addWidget(progressBar, 1, 0, 1, 1);

        TabArea->addTab(SongList, QString());

        gridLayout->addWidget(TabArea, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 814, 21));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        TabArea->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Comm Audio", 0, QApplication::UnicodeUTF8));
        sendButton->setText(QApplication::translate("MainWindow", "Send", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        micRButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        select_URL_Btn->setText(QApplication::translate("MainWindow", "Queue URL", 0, QApplication::UnicodeUTF8));
        connect_Btn->setText(QApplication::translate("MainWindow", "Connect", 0, QApplication::UnicodeUTF8));
        play_Btn->setText(QApplication::translate("MainWindow", "Play", 0, QApplication::UnicodeUTF8));
        download_Btn->setText(QApplication::translate("MainWindow", "Download File", 0, QApplication::UnicodeUTF8));
        pause_Btn->setText(QApplication::translate("MainWindow", "Pause", 0, QApplication::UnicodeUTF8));
        mute_Btn->setText(QApplication::translate("MainWindow", "Mute User", 0, QApplication::UnicodeUTF8));
        select_file_Btn->setText(QApplication::translate("MainWindow", "Queue File", 0, QApplication::UnicodeUTF8));
        skip_for_Btn->setText(QApplication::translate("MainWindow", "Skip Song", 0, QApplication::UnicodeUTF8));
        TabArea->setTabText(TabArea->indexOf(People), QApplication::translate("MainWindow", "People", 0, QApplication::UnicodeUTF8));
        TabArea->setTabText(TabArea->indexOf(SongList), QApplication::translate("MainWindow", "Songs", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
