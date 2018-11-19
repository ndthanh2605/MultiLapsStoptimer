#include "jsonformatter.h"

#include <QJsonArray>

QJsonObject JsonFormatter::makeJsonTimerLap(TimerLap *lap)
{
    QJsonObject obj;
    if (lap) {
        obj["time"] = lap->time();
        obj["label"] = lap->label();
    }
    return obj;
}

TimerLap *JsonFormatter::getTimerLapFromJson(const QJsonObject &obj)
{
    TimerLap *lap = new TimerLap();
    lap->setTime(obj["time"].toInt());
    lap->setLabel(obj["label"].toString());
    return lap;
}

QJsonObject JsonFormatter::makeJsonStoptimer(Stoptimer *stopTimer)
{
    QJsonObject obj;
    QJsonArray arrLaps;
    if (stopTimer) {
        QHash<int, TimerLap *> laps = stopTimer->timerLaps();
        for (auto itr = laps.begin(); itr != laps.end(); itr++) {
            arrLaps.append(makeJsonTimerLap(itr.value()));
        }
    }
    obj["laps"] = arrLaps;
    return obj;
}

Stoptimer *JsonFormatter::getStopTimerFromJson(const QJsonObject &obj)
{
    Stoptimer *rs = new Stoptimer;
    QJsonArray arrLaps = obj["laps"].toArray();
    for (int i = 0; i < arrLaps.size(); ++i) {
        rs->addLap(getTimerLapFromJson(arrLaps.at(i).toObject()));
    }
    return rs;
}
