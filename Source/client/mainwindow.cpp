#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processmic.h"
#include "playaudio.h"
#include "entercatch.h"

ProcessMic mic;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings_window(new Settings(this)),
    cont_thread(nullptr)

{
    //VlcInstance * v = new VlcInstance(VlcCommon::args(), this);
    ui->setupUi(this);
    QAction * settingsAct = ui->menuBar->addAction("Settings");

    connect(settingsAct, SIGNAL(triggered()), this, SLOT(on_open_settings()));
    connect(ui->connect_Btn, SIGNAL(clicked()), this, SLOT(connect_control()));
	connect(ui->pause_Btn, SIGNAL(clicked()), this, SLOT(pause_control()));
	connect(ui->play_Btn, SIGNAL(clicked()), this, SLOT(play_control()));
	connect(ui->skip_for_Btn, SIGNAL(clicked()), this, SLOT(skip_for_control()));
	connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(send_control()));
	connect(ui->mute_Btn, SIGNAL(clicked()), this, SLOT(mute_control()));
	connect(ui->select_file_Btn, SIGNAL(clicked()), this, SLOT(file_select_control()));
	connect(ui->select_URL_Btn, SIGNAL(clicked()), this, SLOT(URL_select_control()));
	connect(ui->download_Btn, SIGNAL(clicked()), this, SLOT(file_download_control()));

    connect(cont_thread, SIGNAL(message_found(message * msg)), this, SLOT(handle_control(msg)));
    connect(cont_thread, SIGNAL(server_disconnect()), this, SLOT(disconnect_server()));
	//VlcMedia("http://incompetech.com/music/royalty-free/mp3-royaltyfree/Who%20Likes%20to%20Party.mp3");
    /*QAudioFormat format;
    audioInput = new QAudioInput();
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);*/
    //ui->pauseButton->setVisible(false);

	chatmodel = new QStandardItemModel();
	usermodel = new QStandardItemModel();
	filesmodel = new QStandardItemModel();
	ui->chat->setModel(chatmodel);
	ui->chat->setEditTriggers((QAbstractItemView::NoEditTriggers));
	ui->list_people->setModel(usermodel);
	ui->list_people->setEditTriggers((QAbstractItemView::NoEditTriggers));
	ui->list_songs->setModel(filesmodel);
	ui->list_songs->setEditTriggers((QAbstractItemView::NoEditTriggers));
	ui->mute_Btn->setEnabled(false);
	ui->pause_Btn->setEnabled(false);
	ui->play_Btn->setEnabled(false);
	ui->select_URL_Btn->setEnabled(false);
	ui->select_file_Btn->setEnabled(false);
	ui->sendButton->setEnabled(false);
	ui->download_Btn->setEnabled(false);
	ui->skip_for_Btn->setEnabled(false);

	ui->lineEdit->installEventFilter((new EnterCatch(this)));

    new PlayAudio();
}

void MainWindow::disconnect_server()
{
    // handle closing thread.
    cont_thread->terminate();
    delete cont_thread;
    delete control;
}

void MainWindow::handle_control(message * msg)
{
    // handle message
    std::cout << msg << std::endl;
}

void MainWindow::connect_control()
{
	static bool con = false;

	if(con == false)
	{
		std::string control_ip   = settings_window->getControlIp().toStdString();
		std::string control_port = settings_window->getControlPort().toStdString();

		control = new Networking(control_ip.c_str(), control_port.c_str());
		if(control->getConnected())
		{
			chatmodel->appendRow(new QStandardItem(QString("Connected!")));
			ui->mute_Btn->setEnabled(true);
			ui->pause_Btn->setEnabled(true);
			ui->play_Btn->setEnabled(true);
			ui->select_file_Btn->setEnabled(true);
			ui->select_URL_Btn->setEnabled(true);
			ui->sendButton->setEnabled(true);
			ui->download_Btn->setEnabled(true);
			ui->skip_for_Btn->setEnabled(true);
			ui->connect_Btn->setText(QString("Disconnect"));
			con = true;
            message msg = {USER_LIST, 0, NULL};
            control->sendMessage(&msg);
            msg = {FILE_LIST, 0, NULL};
            control->sendMessage(&msg);
            cont_thread = new ControlThread(control);
            cont_thread->start();

		}
		else
		{
			chatmodel->appendRow(new QStandardItem(QString("Connection Failed...")));
		}
	}
	else
	{
		delete control;
		ui->mute_Btn->setEnabled(false);
		ui->pause_Btn->setEnabled(false);
		ui->play_Btn->setEnabled(false);
		ui->select_file_Btn->setEnabled(false);
		ui->select_URL_Btn->setEnabled(false);
		ui->sendButton->setEnabled(false);
		ui->download_Btn->setEnabled(false);
		ui->skip_for_Btn->setEnabled(false);
		ui->connect_Btn->setText(QString("Connect"));
		con = false;
		chatmodel->appendRow(new QStandardItem(QString("Disconnected...")));
	}
}

void MainWindow::pause_control()
{
    char m = PAUSE;
    message mes = {PLAYBACK, 1, &m};
    control->sendMessage(&mes);
}

void MainWindow::play_control()
{
    char m = PLAY;
    message mes = {PLAYBACK, 1, &m};
    control->sendMessage(&mes);
}

void MainWindow::skip_for_control()
{
    char m = SKIP;
    message mes = {PLAYBACK, 1, &m};
    control->sendMessage(&mes);
}

void MainWindow::send_control()
{
	QString text = ui->lineEdit->text();
    QString cmessage("You: ");
    cmessage.append(text);


	ui->lineEdit->clear();

	if(text.size() > 0)
	{
        chatmodel->appendRow(new QStandardItem(cmessage));
        message mes = {MESSAGE, text.length(), (char *) text.toStdString().c_str()};
        control->sendMessage(&mes);
	}
}

void MainWindow::mute_control()
{
	QString text = getSelected(ui->list_people);
	if(text == NULL)
		return;
	std::cerr<<"Selected user: "<<text.toStdString().c_str()<<std::endl;

    message mes = {MUTE, text.length(), (char *) text.toStdString().c_str()};
    control->sendMessage(&mes);
}

void MainWindow::file_select_control()
{
	QString text = getSelected(ui->list_songs);
	if(text == NULL)
		return;
	std::cerr<<"Selected file: "<<text.toStdString().c_str()<<std::endl;

	char *b = (char*)malloc(strlen(text.toStdString().c_str()) + 1);
	b[0] = 0;

    message mes = {FILE_SELECT, strlen(text.toStdString().c_str() + 1), b};
    control->sendMessage(&mes);
	free(b);
}

void MainWindow::URL_select_control()
{
	QString text = ui->lineEdit->text();
	ui->lineEdit->clear();

	if(text.size() > 0)
	{
		char *b = (char*)malloc(strlen(text.toStdString().c_str()) + 1);
		b[0] = 1;
        message mes = {FILE_SELECT, text.length() + 1, b};
        control->sendMessage(&mes);
		free(b);
	}
}

void MainWindow::file_download_control()
{
	ui->download_Btn->setEnabled(false);
	QString text = getSelected(ui->list_songs);

	std::cerr<<text.toStdString().c_str();


    message mes = {FILE_REQUEST, text.length(), (char *) text.toStdString().c_str()};
    control->sendMessage(&mes);
}

QString MainWindow::getSelected(QListView *view)
{
	QModelIndexList list = view->selectionModel()->selectedIndexes();
	if(list.empty())
		return NULL;
	return list.first().data().toString();
}

void MainWindow::on_open_settings()
{
    settings_window->exec();
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
   // ui->playButton->setVisible(false);
   // ui->pauseButton->setVisible(true);

}

void MainWindow::on_pauseButton_clicked()
{
    //ui->pauseButton->setVisible(false);
    //ui->playButton->setVisible(true);
}
