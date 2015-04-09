#ifndef PLAYAUDIO
#define PLAYAUDIO

#include <QObject>
#include <QBuffer>
#include <QDebug>
#include <QUdpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QTimer>

#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioFormat>

#include "customwidget.h"

#define BUFF_SIZE 44100

class PlayAudio : public QObject
{
    Q_OBJECT public: explicit PlayAudio(QString audio_ip, int audio_port,
                                        QString mic_recv_ip, int mic_recv_port,
                                        QObject *parent = 0);
    signals:
        void bytesWritten();
        void barsSet(QByteArray);
    public slots:
        void playData();
        void playMic();
        //void playBuff();
		void setVolume(int c);

    private:
        QUdpSocket *socket;
        QUdpSocket *mic_socket;
        QAudioOutput *output;
        QAudioOutput *mic_output;
        QIODevice *device;
        QIODevice *mic_device;

        QByteArray *buffered;
        int buff_pos;

        QByteArray *mic_buffered;
        int mic_buff_pos;

        QTimer *timer;
};

#endif // PLAYAUDIO
