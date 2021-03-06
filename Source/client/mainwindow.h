#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioInput>
#include <QStandardItemModel>
#include <QListView>
#include "settings.h"
#include "networking.h"
#include "controlthread.h"
#include "playaudio.h"

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
	void handle_download(message *msg);
	void handle_eof();

private slots:
	void on_micRButton_toggled(bool checked);

public slots:
	void send_control();
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
	void vol_control(int);
	void handle_file_size(message *msg);

    void on_seek_move();
    void on_seek_pressed();



	void on_playButton_clicked();
	void on_pauseButton_clicked();

private:
     void seek_bar_chaged(message *);
    Ui::MainWindow *ui;
    QAudioInput *audioInput;
	Settings *settings_window;
	Networking *control;
	QStandardItemModel *chatmodel;
	QStandardItemModel *usermodel;
	QStandardItemModel *filesmodel;
	ControlThread *cont_thread;
	PlayAudio *audio;
    CustomWidget *widget;
	HANDLE dlFile;
	int filesize, filereceived;
    bool seeking;
};

#endif // MAINWINDOW_H
