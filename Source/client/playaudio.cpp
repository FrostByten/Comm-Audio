#include "playaudio.h"

PlayAudio::PlayAudio(QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket();
    mic_socket = new QUdpSocket();
    buff_pos = 0;
    buffered = new QByteArray();

    mic_buff_pos = 0;
    mic_buffered = new QByteArray();

    socket->bind(QHostAddress::AnyIPv4, 8910, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    socket->joinMulticastGroup(QHostAddress("234.5.6.7"));

    mic_socket->bind(QHostAddress::AnyIPv4, 8913, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    mic_socket->joinMulticastGroup(QHostAddress("234.5.6.8"));

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

    output = new QAudioOutput(format);
    output->setBufferSize(88400);
    device = output->start();

    mic_output = new QAudioOutput(format);
    mic_output->setBufferSize(88400);
    mic_device = mic_output->start();

    connect(socket, SIGNAL(readyRead()), this, SLOT(playData()));
    connect(mic_socket, SIGNAL(readyRead()), this, SLOT(playMic()));
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
            //qDebug() << "at start buffered size: " << buffered->size() << " Data size: " << data.size();
            device->write(buffered->data(), buffered->size());
            buffered->clear();
            //timer->start();
            buff_pos=0;
        }

    }
}

/*void PlayAudio::playBuff()
{
    if (!buffered->isEmpty())
    {
        qDebug() << "buffered size: " << buffered->size();
        //device->write(*buffered, buffered->size());
        device->write(buffered->data(), buffered->size());
        buffered->clear();
    }
}*/

void PlayAudio::playMic()
{
    if (mic_socket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(mic_socket->pendingDatagramSize());
        mic_socket->readDatagram(data.data(), data.size());
        //mic_device->write(data.data(), data.size());
        if (mic_buff_pos < BUFF_SIZE)
        {
            mic_buffered->append(data.data(),data.size());
            mic_buff_pos+=data.size();
        }
        else
        {
            mic_buffered->append(data.data(),data.size());
            //qDebug() << "at start buffered size: " << buffered->size() << " Data size: " << data.size();
            mic_device->write(mic_buffered->data(), mic_buffered->size());
            mic_buffered->clear();
            //timer->start();
            mic_buff_pos=0;
        }

    }
}

void PlayAudio::setVolume(int c)
{
	qreal v = ((float)c / 99.0f);
	output->setVolume(v);
	mic_output->setVolume(v);
}
