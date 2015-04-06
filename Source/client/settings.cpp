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
