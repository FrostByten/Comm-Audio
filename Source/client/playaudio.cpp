#include "playaudio.h"

PlayAudio::PlayAudio(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket();
    buff_pos = 0;
    buffered = new QByteArray();

    socket->bind(QHostAddress::AnyIPv4, 8910, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    socket->joinMulticastGroup(QHostAddress("234.5.6.7"));

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format))
            format = info.nearestFormat(format);

    timer =  new QTimer (this);
    connect(timer, SIGNAL(timeout()),this, SLOT(playBuff()));
    timer->setInterval(169.5);

    output = new QAudioOutput(format);
    device = output->start();
    connect(socket, SIGNAL(readyRead()), this, SLOT(playData()));
}

void PlayAudio::playData()
{
    QByteArray data;
    if (socket->hasPendingDatagrams())
    {

        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size());

        if (buff_pos < BUFF_SIZE)
        {
            buffered->append(data.data(),data.size());
            buff_pos+=data.size();
        }
        else
        {
            buffered->append(data.data(),data.size());
            qDebug() << "at start buffered size: " << buffered->size() << " Data size: " << data.size();

            timer->start();
            buff_pos=0;
        }

    }
}

void PlayAudio::playBuff()
{
    if (!buffered->isEmpty())
    {
        qDebug() << "buffered size: " << buffered->size();
        //device->write(*buffered, buffered->size());
        device->write(buffered->data(), buffered->size());
        buffered->clear();
    }
}
