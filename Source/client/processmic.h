#ifndef PROCESSMIC
#define PROCESSMIC

#include <QAudioInput>
#include <QUdpSocket>
#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QObject>

class ProcessMic : public QObject
{
  Q_OBJECT
public:
  void startMic();
  void stopMic();

private:
  QFile outputFile;
  QAudioInput *audioInput;

};

#endif // PROCESSMIC

