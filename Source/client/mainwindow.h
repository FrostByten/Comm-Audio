#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioInput>

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

private:
    Ui::MainWindow *ui;
    QAudioInput *audioInput;
};

#endif // MAINWINDOW_H
