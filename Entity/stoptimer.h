#ifndef STOPTIMER_H
#define STOPTIMER_H

#include "timerlap.h"
#include <QHash>

class Stoptimer
{
public:
    Stoptimer();

    void addLap(TimerLap *lap);
    void removeLap(int lapTime);
    void clearLaps();

    QHash<int, TimerLap *> timerLaps() const;
    void setTimerLaps(const QHash<int, TimerLap *> &timerLaps);

private:
    QHash<int, TimerLap *> m_timerLaps;

};

#endif // STOPTIMER_H
