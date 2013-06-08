#include "digitalclock.h"

#include <QtGui>
#include <sstream>

DigitalClock::DigitalClock(QWidget *parent)
    : QLCDNumber(8, parent)
{
    setSegmentStyle(Filled);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    showTime();

    setWindowTitle(tr("Digital Clock"));
    resize(150, 60);

    startTime.start();
}

void DigitalClock::showTime()
{
    const qint64 elapsedms = startTime.elapsed();
    const qint64 elapseds = elapsedms / 1000;
    const qint64 elapsedm = elapseds / 60;
    const qint64 elapsedh = elapsedm / 60;

    std::stringstream ss;
    ss << elapsedh;
    ss << ":" << ((elapsedm%60) < 10 ? "0" : "") << elapsedm%60;
    ss << ":" << ((elapseds%60) < 10 ? "0" : "") << elapseds%60;

    display(QString(ss.str().c_str()));
}
