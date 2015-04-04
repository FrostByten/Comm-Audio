#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioInput>
#include <QStandardItemModel>
#include <QListView>
#include "settings.h"
#include "networking.h"
#include "controlthread.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

	void handle_userList(message *msg);
	void handle_setName(message *msg);

private slots:
    void on_micRButton_toggled(bool checked);

    void on_playButton_clicked();

    void on_pauseButton_clicked();
    void connect_control();
	void pause_control();
	void play_control();
	void skip_for_control();
	void mute_control();
	void file_select_control();
	void URL_select_control();
	void file_download_control();
    void on_open_settings();
	QString getSelected(QListView *);
    void disconnect_server();
    void handle_control(message * msg);
public slots:
	void send_control();


private:
    Ui::MainWindow *ui;
    QAudioInput *audioInput;
    Settings * settings_window;
    Networking * control;
	QStandardItemModel *chatmodel;
	QStandardItemModel *usermodel;
	QStandardItemModel *filesmodel;
    ControlThread * cont_thread;
};

#endif // MAINWINDOW_H
