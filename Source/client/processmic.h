#ifndef PROCESSMIC
#define PROCESSMIC

#include <QtMultimedia/QAudioInput>
#include <QtNetwork/QUdpSocket>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QObject>

class ProcessMic : public QObject
{
  Q_OBJECT
public:
  void startMic(QString host);
  void stopMic();

private:
  QFile outputFile;
  QAudioInput *audioInput;

};

#endif // PROCESSMIC

