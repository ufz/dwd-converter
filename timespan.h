#ifndef TIMESPAN_H
#define TIMESPAN_H

#include <QDate>

class TimeSpan
{
public:
    TimeSpan();
    TimeSpan(QDate startDate, QDate endDate);

    QDate getStartDate();
    QDate getEndDate();

    void setStartDate(QDate date);
    void setEndDate(QDate date);

    bool isEmpty();

    QString toQString();

private:
    QDate startDate;
    QDate endDate;
};

#endif // TIMESPAN_H
