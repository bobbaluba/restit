#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QLCDNumber>
#include <QElapsedTimer>

class DigitalClock : public QLCDNumber
{
    Q_OBJECT
private:
    QElapsedTimer startTime;

public:
    DigitalClock(QWidget *parent = 0);

private slots:
    void showTime();
};

#endif
