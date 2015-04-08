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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
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
    QGridLayout *Settings;
    QWidget *widget;
    QGridLayout *gridLayout_5;
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(814, 435);
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
        horizontalLayout->setSpacing(5);
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

        Settings = new QGridLayout();
        Settings->setSpacing(6);
        Settings->setObjectName(QStringLiteral("Settings"));
        Settings->setSizeConstraint(QLayout::SetDefaultConstraint);
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_5 = new QGridLayout(widget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));

        Settings->addWidget(widget, 0, 0, 1, 4);

        select_URL_Btn = new QPushButton(centralWidget);
        select_URL_Btn->setObjectName(QStringLiteral("select_URL_Btn"));

        Settings->addWidget(select_URL_Btn, 4, 3, 1, 1);

        connect_Btn = new QPushButton(centralWidget);
        connect_Btn->setObjectName(QStringLiteral("connect_Btn"));

        Settings->addWidget(connect_Btn, 4, 0, 1, 1);

        play_Btn = new QPushButton(centralWidget);
        play_Btn->setObjectName(QStringLiteral("play_Btn"));

        Settings->addWidget(play_Btn, 4, 1, 1, 1);

        download_Btn = new QPushButton(centralWidget);
        download_Btn->setObjectName(QStringLiteral("download_Btn"));

        Settings->addWidget(download_Btn, 7, 3, 1, 1);

        pause_Btn = new QPushButton(centralWidget);
        pause_Btn->setObjectName(QStringLiteral("pause_Btn"));

        Settings->addWidget(pause_Btn, 3, 1, 1, 1);

        volume = new QSlider(centralWidget);
        volume->setObjectName(QStringLiteral("volume"));
        volume->setCursor(QCursor(Qt::SplitHCursor));
        volume->setValue(99);
        volume->setOrientation(Qt::Horizontal);

        Settings->addWidget(volume, 7, 1, 1, 1);

        mute_Btn = new QPushButton(centralWidget);
        mute_Btn->setObjectName(QStringLiteral("mute_Btn"));

        Settings->addWidget(mute_Btn, 3, 0, 1, 1);

        select_file_Btn = new QPushButton(centralWidget);
        select_file_Btn->setObjectName(QStringLiteral("select_file_Btn"));

        Settings->addWidget(select_file_Btn, 3, 3, 1, 1);

        skip_for_Btn = new QPushButton(centralWidget);
        skip_for_Btn->setObjectName(QStringLiteral("skip_for_Btn"));

        Settings->addWidget(skip_for_Btn, 7, 0, 1, 1);

        seek_Bar = new QSlider(centralWidget);
        seek_Bar->setObjectName(QStringLiteral("seek_Bar"));
        seek_Bar->setMaximum(999);
        seek_Bar->setOrientation(Qt::Horizontal);

        Settings->addWidget(seek_Bar, 2, 0, 1, 4);

        Settings->setRowStretch(0, 1);

        gridLayout->addLayout(Settings, 0, 2, 1, 1);

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
        list_people = new QListView(People);
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
        list_songs = new QListView(SongList);
        list_songs->setObjectName(QStringLiteral("list_songs"));

        gridLayout_4->addWidget(list_songs, 0, 0, 1, 1);

        progressBar = new QProgressBar(SongList);
        progressBar->setObjectName(QStringLiteral("progressBar"));
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
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 814, 21));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        TabArea->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Comm Audio", 0));
        sendButton->setText(QApplication::translate("MainWindow", "Send", 0));
#ifndef QT_NO_TOOLTIP
        micRButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        select_URL_Btn->setText(QApplication::translate("MainWindow", "Queue URL", 0));
        connect_Btn->setText(QApplication::translate("MainWindow", "Connect", 0));
        play_Btn->setText(QApplication::translate("MainWindow", "Play", 0));
        download_Btn->setText(QApplication::translate("MainWindow", "Download File", 0));
        pause_Btn->setText(QApplication::translate("MainWindow", "Pause", 0));
        mute_Btn->setText(QApplication::translate("MainWindow", "Mute User", 0));
        select_file_Btn->setText(QApplication::translate("MainWindow", "Queue File", 0));
        skip_for_Btn->setText(QApplication::translate("MainWindow", "Skip Song", 0));
        TabArea->setTabText(TabArea->indexOf(People), QApplication::translate("MainWindow", "People", 0));
        TabArea->setTabText(TabArea->indexOf(SongList), QApplication::translate("MainWindow", "Songs", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
