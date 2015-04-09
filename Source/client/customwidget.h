#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QPainter>
#include <QObject>
#include <QWidget>

#define NUMCHANNELS 32

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    CustomWidget();
    ~CustomWidget();

public slots:
    void setBars(QByteArray);

protected:
    void paintEvent(QPaintEvent *event);

private:
    float bars[32];
    float newBars[32];
    QPen pens[32];
};

#endif // CUSTOMWIDGET_H
