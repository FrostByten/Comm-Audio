/*-------------------------------------------------------------------------
-- CLASS: ProcessMic
--
-- DATE: March 16, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- FUNCTIONS:
--      void ProcessMic::startMic(QString host)
--      void ProcessMic::stopMic()
--
-- NOTES:
--      Handles the mic transmission from clients
-------------------------------------------------------------------------*/
#include "processmic.h"

/*-------------------------------------------------------------------------
-- FUNCTION: ProcessMic::startMic
--
-- DATE: March 16, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- INTERFACE: void ProcessMic::startMic(QString host)
--
-- PARAMETERS:
--		QString host - host ip address
--      int port       host port
--
-- RETURNS: void
--
-- NOTES:
--      Starts the mic transmission to the server
-------------------------------------------------------------------------*/
void ProcessMic::startMic(QString host, int port)
{
    // Set up sound format
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning()<<"Using nearest format";
        format = info.nearestFormat(format);
    }

    // create audio input
    audioInput = new QAudioInput(format,reinterpret_cast<QObject*>(this));
    QUdpSocket* socket = new QUdpSocket();
    socket->connectToHost(host, port);

    // starts the audio input and send it through the UDP socket
    audioInput->start(socket);
}

/*-------------------------------------------------------------------------
-- FUNCTION: ProcessMic::stopMic
--
-- DATE: March 16, 2015
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Marc Rafanan
--
-- PROGRAMMER: Marc Rafanan
--
-- INTERFACE: void ProcessMic::stopMic()
--
-- PARAMETERS:
--
-- RETURNS: void
--
-- NOTES:
--      Stops the mic transmission
-------------------------------------------------------------------------*/
void ProcessMic::stopMic()
{
    if(audioInput != NULL)
    {
        audioInput->stop();
        outputFile.close();
        delete audioInput;
    }
}
