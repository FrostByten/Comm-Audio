/*-------------------------------------------------------------------------
-- CLASS: PlayAudio
--
-- DATE: April, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- CONSTRUCTOR:
--      PlayAudio::PlayAudio(QString audio_ip, int audio_port,
--                           QString mic_recv_ip, int mic_recv_port,
--                           QObject *parent)
-- FUNCTIONS:
--      void PlayAudio::playData()
--      void PlayAudio::playMic()
--      void PlayAudio::setVolume(int c)
--
-- NOTES:
--      Handles the mic transmission from clients
-------------------------------------------------------------------------*/
#include "playaudio.h"
#include <iostream>

/*-------------------------------------------------------------------------
-- CONSTRUCTOR: PlayAudio::PlayAudio
--
-- DATE: April 3, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- INTERFACE: PlayAudio::PlayAudio(QString audio_ip, int audio_port,
--                                 QString mic_recv_ip, int mic_recv_port,
--                                 QObject *parent)
--
-- PARAMETERS:
--		QString audio_ip    - multicast ip for the audio
--      int     audio_port  - multicast port for audio
--      QString mic_recv_ip - multicast ip for mic
--      int     mic_recv_port - multicast port for mic
--      QObject *parent     - parent Object (optional)
--
-- RETURNS:
--
-- NOTES:
--      Creates PlayAudio object that plays audio and mic streams
-------------------------------------------------------------------------*/
PlayAudio::PlayAudio(QString audio_ip, int audio_port,
                     QString mic_recv_ip, int mic_recv_port, QObject *parent) : QObject(parent)
{
    socket = new QUdpSocket();
    mic_socket = new QUdpSocket();
    buff_pos = 0;
    buffered = new QByteArray();

    mic_buff_pos = 0;
    mic_buffered = new QByteArray();

    // join to audio multicast
    socket->bind(QHostAddress::AnyIPv4, audio_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    socket->joinMulticastGroup(QHostAddress(audio_ip));

    // join to mic multicast
    mic_socket->bind(QHostAddress::AnyIPv4, mic_recv_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    mic_socket->joinMulticastGroup(QHostAddress(mic_recv_ip));

    // setup audio format
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

    // link audio output to audio device
    output = new QAudioOutput(format);
    output->setBufferSize(BUFF_SIZE * 2);
    device = output->start();

    // set format for mic
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    if (!info.isFormatSupported(format))
            format = info.nearestFormat(format);

    mic_output = new QAudioOutput(format);
    mic_output->setBufferSize(BUFF_SIZE * 2);
    mic_device = mic_output->start();

    // listen to both mic and audio sockets
    connect(socket, SIGNAL(readyRead()), this, SLOT(playData()));
    connect(mic_socket, SIGNAL(readyRead()), this, SLOT(playMic()));
}

/*-------------------------------------------------------------------------
-- FUNCTION: PlayAudio::playData()
--
-- DATE: April 3, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- INTERFACE: void PlayAudio::playData()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--      Slot that activates when there's data available on the audio socket.
--      Buffers incoming data before sending to the output buffer.
-------------------------------------------------------------------------*/
void PlayAudio::playData()
{
    QByteArray data;
    QByteArray barsData;
    int pos = 0;

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
            device->write(buffered->data(), buffered->size());
            buffered->clear();
            buff_pos=0;
        }

        pos += data.size();
        if (pos >= 32)
        {
            barsData.append( buffered->constData()-(((buff_pos-pos) >= 32)?(buff_pos-pos):(buff_pos)));
            //std::cerr << pos << " " << buff_pos << " " << barsData.size() << " " << (buff_pos-pos) << " " << buffered->constData()+buff_pos-pos << std::endl;
            //barsData.chop(pos - );
            if (barsData.size() >= 32)
            {
                emit barsSet(barsData);
            }
            pos = 0;
        }
    }
}

/*-------------------------------------------------------------------------
-- FUNCTION: PlayAudio::playMic()
--
-- DATE: April 3, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- INTERFACE: void PlayAudio::playMic()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--      Slot that activates when there's data available on the mic socket.
--      Buffers incoming data before sending to the output buffer.
-------------------------------------------------------------------------*/
void PlayAudio::playMic()
{
    /*QByteArray barsData;
    int pos = 0;*/
    if (mic_socket->hasPendingDatagrams())
    {
        QByteArray data;
        data.resize(mic_socket->pendingDatagramSize());
        mic_socket->readDatagram(data.data(), data.size());

        if (mic_buff_pos < BUFF_SIZE)
        {
            mic_buffered->append(data.data(),data.size());
            mic_buff_pos+=data.size();
        }
        else
        {
            mic_buffered->append(data.data(),data.size());
            mic_device->write(mic_buffered->data(), mic_buffered->size());
            mic_buffered->clear();
            mic_buff_pos=0;
        }
        /*pos += data.size();
        if (pos >= 32)
        {
            barsData = barsData.replace(0, pos, buffered->constData()-pos);
            if (barsData.size() >= 32)
            {
                emit barsSet(barsData);
            }
            pos = 0;
        }*/
    }
}

/*-------------------------------------------------------------------------
-- FUNCTION: PlayAudio::setVolume
--
-- DATE: April 3, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Lewis Scott
--
-- PROGRAMMER: Lewis Scott
--
-- INTERFACE: void PlayAudio::setVolume(int c)
--
-- PARAMETERS:
--          int c - volume setting
--
-- RETURNS: void
--
-- NOTES:
--      Controls the volume for both audio and mic
-------------------------------------------------------------------------*/
void PlayAudio::setVolume(int c)
{
	qreal v = ((float)c / 99.0f);
	output->setVolume(v);
	mic_output->setVolume(v);
}
