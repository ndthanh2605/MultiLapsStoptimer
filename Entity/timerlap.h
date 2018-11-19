#ifndef TIMERLAP_H
#define TIMERLAP_H

#include <QString>

class TimerLap
{
public:
    TimerLap();
    TimerLap(int time, QString label);

    QString label() const;
    void setLabel(const QString &label);

    int time() const;
    void setTime(int time);

private:
    int m_time = 0;
    QString m_label;

};

#endif // TIMERLAP_H
