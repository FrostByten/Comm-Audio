#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processmic.h"
#include "playaudio.h"
#include "entercatch.h"
#include "controlthread.h"
#include "customwidget.h"

ProcessMic mic;

/*-------------------------------------------------------------------------
-- FUNCTION: MainWindow::MainWindow(QWidget *parent)
--
-- DATE: March 15th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: MainWindow::MainWindow(QWidget *parent)
--
-- PARAMETERS: QWidget : parent widget.
--
-- RETURNS: void
--
-- NOTES:
--     This function contructs the main window for the project.
-------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings_window(new Settings(this)),
    cont_thread(nullptr)
{
	audio = NULL;

    //VlcInstance * v = new VlcInstance(VlcCommon::args(), this);
    ui->setupUi(this);
    QAction * settingsAct = ui->menuBar->addAction("Settings");

    widget = new CustomWidget();
    ui->BarGraph->addWidget(widget);

	filereceived = 0;

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
	connect(ui->volume, SIGNAL(sliderMoved(int)), this, SLOT(vol_control(int)));
    connect(ui->seek_Bar, SIGNAL(sliderReleased()), this, SLOT(on_seek_move()));
    connect(ui->seek_Bar, SIGNAL(sliderPressed()), this, SLOT(on_seek_pressed()));

	filesize = 1;

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
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::on_seek_pressed()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::on_seek_pressed()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function contructs the main window for the project.
-------------------------------------------------------------------------*/
void MainWindow::on_seek_pressed()
{
    seeking = true;
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::disconnect_server()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::disconnect_server()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function contructs the main window for the project.
-------------------------------------------------------------------------*/
void MainWindow::disconnect_server()
{
    // handle closing thread.
	connect_control();
}

void MainWindow::handle_control(message * msg)
{
	switch(msg->type)
	{
		case MESSAGE:
		{
			chatmodel->appendRow(new QStandardItem(QString(msg->data)));
			break;
		}
		case USER_LIST:
		{
			handle_userList(msg);
			break;
		}
		case FILE_LIST:
		{
			filesmodel->appendRow(new QStandardItem(QString(msg->data)));
			break;
		}
		case CURRENT:
		{
			QString m("Now playing: ");
			m.append(msg->data);
			chatmodel->appendRow(new QStandardItem(m));
			break;
		}
		case SET_NAME:
		{
			handle_setName(msg);
			break;
		}
		case FILE_REQUEST:
		{
			handle_download(msg);
			break;
		}
		case FILE_EOF:
		{
			handle_eof();
			break;
		}
		case FILE_SIZE:
		{
			handle_file_size(msg);
			break;
		}
        case SEEK:
        {
            seek_bar_chaged(msg);
            break;
        }
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::on_seek_pressed()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: MainWindow::MainWindow(QWidget *parent)
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function contructs the main window for the project.
-------------------------------------------------------------------------*/
void MainWindow::handle_userList(message *msg)
{
	if(msg->data[0] == EXIST || msg->data[0] == CONNECT)
		usermodel->appendRow(new QStandardItem(QString(msg->data+1)));
	if(msg->data[0] == CONNECT)
	{
		QString m(msg->data+1);
		m.append(" has connected");
		chatmodel->appendRow(new QStandardItem(m));
	}
	if(msg->data[0] == DISCONNECT)
	{
		QString m(msg->data+1);

		for(int i = 0; i < usermodel->rowCount(); ++i)
		{
			if(usermodel->index(i,0).data().toString().compare(m) == 0)
				usermodel->removeRow(i);
		}
		m.append(" disconnected");
		chatmodel->appendRow(new QStandardItem(m));
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::handle_setName(message *msg)
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: MainWindow::MainWindow(QWidget *parent)
--
-- PARAMETERS: message * msg
--
-- RETURNS: void
--
-- NOTES:
--     This function contructs the main window for the project.
-------------------------------------------------------------------------*/
void MainWindow::handle_setName(message *msg)
{
	int j = 0;
	for(;;++j)
	{
        if(msg->data[j] == '\0')
		{
			break;
		}
	}

	for(int i = 0; i < usermodel->rowCount(); ++i)
	{
		if(usermodel->index(i,0).data().toString().compare(msg->data) == 0)
			usermodel->setItem(i, 0, new QStandardItem(msg->data+j));
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::connect_control()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::connect_control()
--
-- PARAMETERS: message * msg
--
-- RETURNS: void
--
-- NOTES:
--     This function contructs the main window for the project.
-------------------------------------------------------------------------*/
void MainWindow::connect_control()
{
	static bool con = false;

	ui->progressBar->setValue(0);

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

			cont_thread = new ControlThread(control);
			connect(cont_thread, SIGNAL(message_found(message *)), this, SLOT(handle_control(message *)));
			connect(cont_thread, SIGNAL(server_disconnect()), this, SLOT(disconnect_server()));
			cont_thread->start();

            message msg = {USER_LIST, 0, NULL};
            control->sendMessage(&msg);
            message msg2 = {FILE_LIST, 0, NULL};
            control->sendMessage(&msg2);

            audio = new PlayAudio(settings_window->getAudioIp(), settings_window->getAudioPort(),
                                  settings_window->getMicRecvIp(), settings_window->getMicRecvPort());
            connect(audio, SIGNAL(barsSet(QByteArray)), widget, SLOT(setBars(QByteArray)));
		}
		else
		{
			chatmodel->appendRow(new QStandardItem(QString("Connection Failed...")));
		}
	}
	else
	{
		delete control;
		delete audio;
		cont_thread->terminate();
		delete cont_thread;
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
		usermodel->clear();
		filesmodel->clear();
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::pause_control()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::pause_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     this function is a slot called by the client to pause audio playback.
-------------------------------------------------------------------------*/
void MainWindow::pause_control()
{
    char m = PAUSE;
    message mes = {PLAYBACK, 1, &m};
    control->sendMessage(&mes);
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::play_control()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::play_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function is called on the client to resume playback on the server.
-------------------------------------------------------------------------*/
void MainWindow::play_control()
{
    char m = PLAY;
    message mes = {PLAYBACK, 1, &m};
    control->sendMessage(&mes);
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::skip_for_control()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::play_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function is called on the client to resume playback on the
--     server.
-------------------------------------------------------------------------*/
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
		message mes = {MESSAGE, text.length(), (char *)text.toStdString().c_str()};
        control->sendMessage(&mes);
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::mute_control()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::mute_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This method is called when the mute command is called on the
--     client. It will send a message to the server. If the user is
--     an admin it will mute the person they currently have selected.
-------------------------------------------------------------------------*/
void MainWindow::mute_control()
{
	QString text = getSelected(ui->list_people);
	if(text == NULL)
		return;
	std::cerr<<"Selected user: "<<text.toStdString().c_str()<<std::endl;

    message mes = {MUTE, text.length(), (char *) text.toStdString().c_str()};
    control->sendMessage(&mes);
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::file_select_control()
--
-- DATE: April 8th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--              Lewis Scott
--
-- INTERFACE: void MainWindow::file_select_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function is called when the user tries to send files to the
--     server. It will queue up a file to play in the current song queue
-------------------------------------------------------------------------*/
void MainWindow::file_select_control()
{
	QString text = getSelected(ui->list_songs);
	if(text == NULL)
		return;

	char *b = (char*)malloc(strlen(text.toStdString().c_str()) + 1);
	b[0] = 1;
	memcpy(b + 1, text.toStdString().c_str(), strlen(text.toStdString().c_str()));

	message mes = {FILE_SELECT, strlen(text.toStdString().c_str()) + 1, b};
    control->sendMessage(&mes);
	free(b);
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::file_select_control()
--
-- DATE: April 7th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--              Lewis Scott
--
-- INTERFACE: void MainWindow::file_select_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function is called when the user tries to send files to the
--     server. It will queue up a file to play in the current song queue
-------------------------------------------------------------------------*/
void MainWindow::on_seek_move()
{
    float value = (float) ui->seek_Bar->value() / (float) ui->seek_Bar->maximum();
    char  currentVol[sizeof(float)];

    memcpy(currentVol, &value, sizeof(value));
    message msg = {SEEK, sizeof(float), currentVol };
    control->sendMessage(&msg);
    seeking = false;
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::seek_bar_chaged(message * msg)
--
-- DATE: April 6th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::file_select_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function is called when the user tries to send files to the
--     server. It will queue up a file to play in the current song queue
-------------------------------------------------------------------------*/
void MainWindow::seek_bar_chaged(message * msg)
{
    if(!seeking)
    {
        float value;
        memcpy(&value, msg->data, sizeof(float));

        ui->seek_Bar->setValue(ui->seek_Bar->maximum() * value);
    }
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::URL_select_control()
--
-- DATE: April 6th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::URL_select_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function requests a URL to be played streaming from the server.
-------------------------------------------------------------------------*/
void MainWindow::URL_select_control()
{
	QString text = ui->lineEdit->text();
	ui->lineEdit->clear();

	if(text.size() > 0)
	{
		char *b = (char*)malloc(strlen(text.toStdString().c_str()) + 1);
		b[0] = 0;
		memcpy(b + 1, text.toStdString().c_str(), strlen(text.toStdString().c_str()));
        message mes = {FILE_SELECT, text.length() + 1, b};
        control->sendMessage(&mes);
		free(b);
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::file_download_control()
--
-- DATE: April 6th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::file_download_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function downloads a file into your current folder, from the
--     server based on what you've currently selected.
-------------------------------------------------------------------------*/
void MainWindow::file_download_control()
{
	ui->download_Btn->setEnabled(false);
	QString text = getSelected(ui->list_songs);

	if(text != NULL)
	{
		if(dlFile != NULL)
			CloseHandle(dlFile);
		wchar_t wtext[1024];
		char *m = (char*)malloc(1024);
		DWORD num = GetCurrentDirectory(1024-text.size(), wtext);
		memcpy(m, text.toStdString().c_str(), text.size());
		m[text.size()] = '\0';
		mbstowcs(wtext + num, m, strlen(m)+1);
		std::cerr << "Requesting: " << m << std::endl;
		dlFile = CreateFile(wtext, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		free(m);

		if(dlFile == NULL || dlFile == INVALID_HANDLE_VALUE)
		{
			chatmodel->appendRow(new QStandardItem(QString("Unable to create local file...")));
			return;
		}
		chatmodel->appendRow(new QStandardItem(QString("Starting file transfer...")));

		m = (char*)malloc(text.length());
		memcpy(m, text.toStdString().c_str(), text.length());
		message mes = {FILE_REQUEST, text.length(), m};
		control->sendMessage(&mes);
		free(m);
	}
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::file_download_control()
--
-- DATE: April 6th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::file_download_control()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function downloads a file into your current folder, from the
--     server based on what you've currently selected.
-------------------------------------------------------------------------*/
void MainWindow::handle_download(message *msg)
{
	if(dlFile != INVALID_HANDLE_VALUE && dlFile != NULL)
	{
		DWORD written = 0;
		WriteFile(dlFile, msg->data, msg->len, &written, NULL);
		if(written != msg->len)
			std::cerr << "Error occurred writing to file!" << std::endl;
		filereceived += msg->len;
		ui->progressBar->setValue((int)(((float)filereceived/(float)filesize)*(float)100));
	}
	else
		std::cerr << "File is not open!" << std::endl;
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::handle_eof()
--
-- DATE: April 6th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::handle_eof()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This file appends an EOF to the end of a file after writing it.
--      and notifies the user that the file has finished downloading.
--
-------------------------------------------------------------------------*/
void MainWindow::handle_eof()
{
	CloseHandle(dlFile);
	dlFile = NULL;
	chatmodel->appendRow(new QStandardItem(QString("File download complete!")));
	ui->download_Btn->setEnabled(true);
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::handle_eof()
--
-- DATE: April 6th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::handle_eof()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This file appends an EOF to the end of a file after writing it.
--      and notifies the user that the file has finished downloading.
--
-------------------------------------------------------------------------*/
QString MainWindow::getSelected(QListView *view)
{
	QModelIndexList list = view->selectionModel()->selectedIndexes();
	if(list.empty())
		return NULL;
	return list.first().data().toString();
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::handle_file_size(message *msg)
--
-- DATE: April 6th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::handle_file_size(message *msg)
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This message copies the file size
--
-------------------------------------------------------------------------*/
void MainWindow::handle_file_size(message *msg)
{
	memcpy(&filesize, msg->data, sizeof(int));
}

/*-------------------------------------------------------------------------
-- FUNCTION: void MainWindow::on_open_settings()
--
-- DATE: April 5th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::on_open_settings()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     This function opens the settings window for connecting to the server.
--
-------------------------------------------------------------------------*/
void MainWindow::on_open_settings()
{
    settings_window->exec();

    /*message msg;
	char *nam = (char*)malloc(strlen(settings_window->getUsername().toStdString().c_str()));
	strcpy(nam, settings_window->getUsername().toStdString().c_str());
    msg.data = nam;
    msg.len = strlen(nam);
    msg.type = SET_NAME;
    control->sendMessage(&msg);*/
}

/*-------------------------------------------------------------------------
-- FUNCTION: MainWindow::~MainWindow()
--
-- DATE: April 5th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: QT Generated
--
-- PROGRAMMER: QT Generated.
--
-- INTERFACE: MainWindow::~MainWindow()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--     deletes the UI
--
-------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*-------------------------------------------------------------------------
-- FUNCTION: MainWindow::~MainWindow()
--
-- DATE: April 5th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- INTERFACE: void MainWindow::on_micRButton_toggled(bool checked)
--
-- PARAMETERS: bool checked: if the microphone should be on or not.
--
-- RETURNS: void
--
-- NOTES:
--     this starts the microphone playback.
--
-------------------------------------------------------------------------*/
void MainWindow::on_micRButton_toggled(bool checked)
{
    if(checked)
        mic.startMic(settings_window->getControlIp(), settings_window->getMicPort());
    else
        mic.stopMic();
}

/*-------------------------------------------------------------------------
-- FUNCTION: MainWindow::~MainWindow()
--
-- DATE: April 5th
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Vouve
--
-- PROGRAMMER: Marc Vouve
--
-- INTERFACE: void MainWindow::vol_control(int c)
--
-- PARAMETERS: int c the volume to play the sound back at.
--
-- RETURNS: void
--
-- NOTES:
--     This function controls audio playback
--
-------------------------------------------------------------------------*/
void MainWindow::vol_control(int c)
{
	if(audio != NULL)
		audio->setVolume(c);
}


