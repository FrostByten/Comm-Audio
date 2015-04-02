#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioInput>
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void on_micRButton_toggled(bool checked);

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_open_settings();

private:
    Ui::MainWindow *ui;
    QAudioInput *audioInput;
    Settings * settings_window;
};

#endif // MAINWINDOW_H
