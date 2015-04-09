#include "customwidget.h"
#include <iostream>
#include <QList>
#include "fft.h"

CustomWidget::CustomWidget()
{
    for (int i = 0; i < 10; i++)
    {
        pens[i] = QPen(QColor::fromRgb(250-(i*25), 0+(i*25), 0), 8);
    }
    for (int i = 10; i < 21; i++)
    {
        pens[i] = QPen(QColor::fromRgb(0, 250-((i-10)*25), 0+((i-10)*25)), 8);
    }
    for (int i = 21; i < 32; i++)
    {
        pens[i] = QPen(QColor::fromRgb(0+((i-21)*25), 0, 250-((i-21)*25)), 8);
    }

    for (int i = 0; i < 32; i++)
    {
        bars[i] = i*16.0F;
    }
}

CustomWidget::~CustomWidget()
{

}

void CustomWidget::setBars(QByteArray data)
{
    float _waveLeft[NUMCHANNELS];
    float _waveRight[NUMCHANNELS];
    float _fftLeft[NUMCHANNELS];
    float _fftRight[NUMCHANNELS];
    float *avgData = new float[data.size()];
    int counter = 0;
    memset(avgData, 0, data.size());

    // Average
    for (int i = 0; i < data.size(); i += NUMCHANNELS)
    {
        for (int j = 0; j < NUMCHANNELS && i+j < data.size(); j++)
        {
            avgData[j] += (float)*((short*)(data.data() + i+j));
        }
        counter = i;
    }

    for (int i = 0; i < NUMCHANNELS; i++)
    {
        avgData[i] = avgData[i] / counter;
    }

    // NUMCHANNELS 32
    for (int i = 0; i < NUMCHANNELS/2; i++)
    {
        _waveLeft[i] = avgData[i];
    }

    for (int i = NUMCHANNELS/2; i < NUMCHANNELS; i++)
    {
        _waveRight[i-(NUMCHANNELS/2)] = avgData[i];
    }

    R16SRFFT(_waveLeft, _fftLeft);
    R16SRFFT(_waveRight, _fftRight);

    for (int i = 0; i < NUMCHANNELS/2; i++)
    {
        newBars[i] = _fftLeft[i];
    }

    for (int i = NUMCHANNELS/2; i < NUMCHANNELS; i++)
    {
        newBars[i] = _fftRight[i-(NUMCHANNELS/2)];
    }

    this->update();
    delete avgData;
}

void CustomWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

   /* for (int j = 0; j < 3; j++)
    {*/
        for (int i = 0; i < NUMCHANNELS; i++)
        {
            painter.setPen(pens[i]);

            bars[i] = (newBars[i] + bars[i]) / 2;

            painter.drawLine(0, (12*i)+15, bars[i], (12*i)+15);

            //bars[i] = newBars[i];
        }
        /*bars[j] = newBars[j];
    }*/
}

