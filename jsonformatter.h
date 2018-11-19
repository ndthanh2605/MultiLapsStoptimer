#ifndef JSONFORMATTER_H
#define JSONFORMATTER_H

#include <QJsonObject>

#include <Entity/stoptimer.h>
#include <Entity/timerlap.h>

class JsonFormatter
{
public:
    static QJsonObject makeJsonTimerLap(TimerLap *lap);
    static TimerLap * getTimerLapFromJson(const QJsonObject &obj);

    static QJsonObject makeJsonStoptimer(Stoptimer *stopTimer);
    static Stoptimer * getStopTimerFromJson(const QJsonObject &obj);

};

#endif // JSONFORMATTER_H
