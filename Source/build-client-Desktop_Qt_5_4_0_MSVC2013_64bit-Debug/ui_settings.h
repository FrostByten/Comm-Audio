/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

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
            Settings->setObjectName(QStringLiteral("Settings"));
        Settings->resize(393, 205);
        buttonBox = new QDialogButtonBox(Settings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(300, 10, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(Settings);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(60, 120, 271, 51));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        multicast_port = new QLineEdit(gridLayoutWidget);
        multicast_port->setObjectName(QStringLiteral("multicast_port"));

        gridLayout->addWidget(multicast_port, 0, 1, 1, 1);

        control_port = new QLineEdit(gridLayoutWidget);
        control_port->setObjectName(QStringLiteral("control_port"));

        gridLayout->addWidget(control_port, 0, 3, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 2, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        mic_port = new QLineEdit(gridLayoutWidget);
        mic_port->setObjectName(QStringLiteral("mic_port"));

        gridLayout->addWidget(mic_port, 1, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 1, 2, 1, 1);

        mic_recv_port = new QLineEdit(gridLayoutWidget);
        mic_recv_port->setObjectName(QStringLiteral("mic_recv_port"));

        gridLayout->addWidget(mic_recv_port, 1, 3, 1, 1);

        gridLayoutWidget_2 = new QWidget(Settings);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 10, 271, 101));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(gridLayoutWidget_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_2->addWidget(label_8, 2, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 3, 0, 1, 1);

        username = new QLineEdit(gridLayoutWidget_2);
        username->setObjectName(QStringLiteral("username"));

        gridLayout_2->addWidget(username, 0, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        audio_ip = new QLineEdit(gridLayoutWidget_2);
        audio_ip->setObjectName(QStringLiteral("audio_ip"));

        gridLayout_2->addWidget(audio_ip, 1, 2, 1, 1);

        control_ip = new QLineEdit(gridLayoutWidget_2);
        control_ip->setObjectName(QStringLiteral("control_ip"));

        gridLayout_2->addWidget(control_ip, 3, 2, 1, 1);

        mic_recv_ip = new QLineEdit(gridLayoutWidget_2);
        mic_recv_ip->setObjectName(QStringLiteral("mic_recv_ip"));

        gridLayout_2->addWidget(mic_recv_ip, 2, 2, 1, 1);


        retranslateUi(Settings);
        QObject::connect(buttonBox, SIGNAL(accepted()), Settings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Settings, SLOT(reject()));

        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Dialog", 0));
        multicast_port->setText(QApplication::translate("Settings", "8910", 0));
        control_port->setText(QApplication::translate("Settings", "8911", 0));
        label->setText(QApplication::translate("Settings", "Control Port", 0));
        label_2->setText(QApplication::translate("Settings", "Audio Port", 0));
        label_3->setText(QApplication::translate("Settings", "Mic Port", 0));
        mic_port->setText(QApplication::translate("Settings", "8912", 0));
        label_7->setText(QApplication::translate("Settings", "Mic Recv Port", 0));
        mic_recv_port->setText(QApplication::translate("Settings", "8913", 0));
        label_8->setText(QApplication::translate("Settings", "Mic Recv IP", 0));
        label_5->setText(QApplication::translate("Settings", "Audio IP", 0));
        label_6->setText(QApplication::translate("Settings", "Control IP", 0));
        username->setText(QApplication::translate("Settings", "Unnamed", 0));
        label_4->setText(QApplication::translate("Settings", "User Name", 0));
        audio_ip->setText(QApplication::translate("Settings", "224.0.0.0", 0));
        control_ip->setText(QApplication::translate("Settings", "192.168.0.8", 0));
        mic_recv_ip->setText(QApplication::translate("Settings", "234.5.6.8", 0));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
