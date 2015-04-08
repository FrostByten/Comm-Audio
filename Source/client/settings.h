#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    QString getControlPort();
    QString getControlIp();
    QString getAudioIp();
    int getAudioPort();
    int getMicPort();
    QString getMicRecvIp();
    int getMicRecvPort();
    QString getUsername();
    ~Settings();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
