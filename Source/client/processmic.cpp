#include "processmic.h"
#include <QDebug>
void ProcessMic::startMic()
{
    // Set up sound format
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning()<<"default format not supported try to use nearest";
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format,reinterpret_cast<QObject*>(this));
    QUdpSocket* socket = new QUdpSocket();
    socket->connectToHost("127.0.0.1", 8912);

    audioInput->start(socket);

}

void ProcessMic::stopMic()
{
    if(audioInput != NULL)
    {
        audioInput->stop();
        outputFile.close();
        delete audioInput;
    }
}
