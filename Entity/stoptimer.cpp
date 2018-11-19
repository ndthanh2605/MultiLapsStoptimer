#include "stoptimer.h"

Stoptimer::Stoptimer()
{

}

void Stoptimer::addLap(TimerLap *lap)
{
    if (lap)
        m_timerLaps.insert(lap->time(), lap);
}

void Stoptimer::removeLap(int lapTime)
{
    TimerLap *lap = m_timerLaps.take(lapTime);
    if (lap)
        delete lap;
}

void Stoptimer::clearLaps()
{
    qDeleteAll(m_timerLaps);
    m_timerLaps.clear();
}

QHash<int, TimerLap *> Stoptimer::timerLaps() const
{
    return m_timerLaps;
}

void Stoptimer::setTimerLaps(const QHash<int, TimerLap *> &timerLaps)
{
    m_timerLaps = timerLaps;
}
