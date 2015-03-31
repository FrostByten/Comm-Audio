#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processmic.h"

ProcessMic mic;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*QAudioFormat format;
    audioInput = new QAudioInput();
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);*/
    ui->pauseButton->setVisible(false);

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

void MainWindow::on_playButton_clicked()
{
    ui->playButton->setVisible(false);
    ui->pauseButton->setVisible(true);

}

void MainWindow::on_pauseButton_clicked()
{
    ui->pauseButton->setVisible(false);
    ui->playButton->setVisible(true);
}
