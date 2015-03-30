#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processmic.h"

ProcessMic mic;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_micRButton_toggled(bool checked)
{
    if(checked)
        mic.startMic();
    else
        mic.stopMic();
}
