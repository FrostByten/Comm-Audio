#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processmic.h"
#include "playaudio.h"
#include "entercatch.h"

ProcessMic mic;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings_window(new Settings(this))

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
			control->sendMessage(USER_LIST, 0, NULL);
			control->sendMessage(FILE_LIST, 0, NULL);
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
	char mes = PAUSE;
	control->sendMessage(PLAYBACK, 1, &mes);
}

void MainWindow::play_control()
{
	char mes = PLAY;
	control->sendMessage(PLAYBACK, 1, &mes);
}

void MainWindow::skip_for_control()
{
	char mes = SKIP;
	control->sendMessage(PLAYBACK, 1, &mes);
}

void MainWindow::send_control()
{
	QString text = ui->lineEdit->text();
	QString message("You: ");
	message.append(text);

	ui->lineEdit->clear();

	if(text.size() > 0)
	{
		chatmodel->appendRow(new QStandardItem(message));

		control->sendMessage(MESSAGE, strlen(text.toStdString().c_str()), text.toStdString().c_str());
	}
}

void MainWindow::mute_control()
{
	QString text = getSelected(ui->list_people);
	if(text == NULL)
		return;
	std::cerr<<"Selected user: "<<text.toStdString().c_str()<<std::endl;

	control->sendMessage(MUTE, strlen(text.toStdString().c_str()), text.toStdString().c_str());
}

void MainWindow::file_select_control()
{
	QString text = getSelected(ui->list_songs);
	if(text == NULL)
		return;
	std::cerr<<"Selected file: "<<text.toStdString().c_str()<<std::endl;

	char *b = (char*)malloc(strlen(text.toStdString().c_str()) + 1);
	b[0] = 0;
	control->sendMessage(FILE_SELECT, strlen(text.toStdString().c_str() + 1), b);
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
		control->sendMessage(FILE_SELECT, strlen(text.toStdString().c_str()) + 1, b);
		free(b);
	}
}

void MainWindow::file_download_control()
{
	ui->download_Btn->setEnabled(false);
	QString text = getSelected(ui->list_songs);

	std::cerr<<text.toStdString().c_str();

	control->sendMessage(FILE_REQUEST, strlen(text.toStdString().c_str()), text.toStdString().c_str());
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
