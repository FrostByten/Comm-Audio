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

#define BUFF_SIZE 44100

class PlayAudio : public QObject
{
    Q_OBJECT public: explicit PlayAudio(QObject *parent = 0);
    signals:
        void bytesWritten();
    public slots:
        void playData();
        //void playBuff();

    private:
        QUdpSocket *socket;
        QAudioOutput *output;
        QIODevice *device;

        //QDataStream *in;
        QByteArray *buffered;
        QBuffer *buffer;
        int buff_pos;
        QTimer *timer;
};

#endif // PLAYAUDIO
