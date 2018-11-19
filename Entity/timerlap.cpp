#include "timerlap.h"

TimerLap::TimerLap()
{

}

TimerLap::TimerLap(int time, QString label)
{
    m_time = time;
    m_label = label;
}

QString TimerLap::label() const
{
    return m_label;
}

void TimerLap::setLabel(const QString &label)
{
    m_label = label;
}

int TimerLap::time() const
{
    return m_time;
}

void TimerLap::setTime(int time)
{
    m_time = time;
}
