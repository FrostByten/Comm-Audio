/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created: Wed Apr 8 19:33:59 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *multicast_port;
    QLineEdit *control_port;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *mic_port;
    QLabel *label_7;
    QLineEdit *mic_recv_port;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_8;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *username;
    QLabel *label_4;
    QLineEdit *audio_ip;
    QLineEdit *control_ip;
    QLineEdit *mic_recv_ip;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QString::fromUtf8("Settings"));
        Settings->resize(393, 205);
        buttonBox = new QDialogButtonBox(Settings);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(300, 10, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(Settings);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(60, 120, 271, 51));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        multicast_port = new QLineEdit(gridLayoutWidget);
        multicast_port->setObjectName(QString::fromUtf8("multicast_port"));

        gridLayout->addWidget(multicast_port, 0, 1, 1, 1);

        control_port = new QLineEdit(gridLayoutWidget);
        control_port->setObjectName(QString::fromUtf8("control_port"));

        gridLayout->addWidget(control_port, 0, 3, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 2, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        mic_port = new QLineEdit(gridLayoutWidget);
        mic_port->setObjectName(QString::fromUtf8("mic_port"));

        gridLayout->addWidget(mic_port, 1, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 1, 2, 1, 1);

        mic_recv_port = new QLineEdit(gridLayoutWidget);
        mic_recv_port->setObjectName(QString::fromUtf8("mic_recv_port"));

        gridLayout->addWidget(mic_recv_port, 1, 3, 1, 1);

        gridLayoutWidget_2 = new QWidget(Settings);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 10, 271, 101));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(gridLayoutWidget_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 2, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 3, 0, 1, 1);

        username = new QLineEdit(gridLayoutWidget_2);
        username->setObjectName(QString::fromUtf8("username"));

        gridLayout_2->addWidget(username, 0, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        audio_ip = new QLineEdit(gridLayoutWidget_2);
        audio_ip->setObjectName(QString::fromUtf8("audio_ip"));

        gridLayout_2->addWidget(audio_ip, 1, 2, 1, 1);

        control_ip = new QLineEdit(gridLayoutWidget_2);
        control_ip->setObjectName(QString::fromUtf8("control_ip"));

        gridLayout_2->addWidget(control_ip, 3, 2, 1, 1);

        mic_recv_ip = new QLineEdit(gridLayoutWidget_2);
        mic_recv_ip->setObjectName(QString::fromUtf8("mic_recv_ip"));

        gridLayout_2->addWidget(mic_recv_ip, 2, 2, 1, 1);


        retranslateUi(Settings);
        QObject::connect(buttonBox, SIGNAL(accepted()), Settings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Settings, SLOT(reject()));

        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Dialog", 0, QApplication::UnicodeUTF8));
        multicast_port->setText(QApplication::translate("Settings", "8910", 0, QApplication::UnicodeUTF8));
        control_port->setText(QApplication::translate("Settings", "8911", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Settings", "Control Port", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Settings", "Audio Port", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Settings", "Mic Port", 0, QApplication::UnicodeUTF8));
        mic_port->setText(QApplication::translate("Settings", "8912", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Settings", "Mic Recv Port", 0, QApplication::UnicodeUTF8));
        mic_recv_port->setText(QApplication::translate("Settings", "8913", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Settings", "Mic Recv IP", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Settings", "Audio IP", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Settings", "Control IP", 0, QApplication::UnicodeUTF8));
        username->setText(QApplication::translate("Settings", "Unnamed", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Settings", "User Name", 0, QApplication::UnicodeUTF8));
        audio_ip->setText(QApplication::translate("Settings", "224.0.0.0", 0, QApplication::UnicodeUTF8));
        control_ip->setText(QApplication::translate("Settings", "192.168.0.8", 0, QApplication::UnicodeUTF8));
        mic_recv_ip->setText(QApplication::translate("Settings", "234.5.6.8", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
