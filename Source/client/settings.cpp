#include "settings.h"
#include "ui_settings.h"

#include <iostream>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
	ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

QString Settings::getControlPort()
{

    return ui->control_port->text();
}

QString Settings::getControlIp()
{
    return ui->control_ip->text();
}

QString Settings::getUsername()
{
	return ui->username->text();
}

QString Settings::getAudioIp()
{
    return ui->audio_ip->text();
}

int Settings::getAudioPort()
{
    return (ui->multicast_port->text()).toInt();
}

int Settings::getMicPort()
{
    return (ui->mic_port->text()).toInt();
}

QString Settings::getMicRecvIp()
{
    return ui->mic_recv_ip->text();
}

int Settings::getMicRecvPort()
{
    return (ui->mic_recv_port->text()).toInt();
}

