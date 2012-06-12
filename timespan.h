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

    QDate setStartDate(QDate date);
    QDate setEndDate(QDate date);

    bool isEmpty();

    QString toQString();

private:
    QDate startDate;
    QDate endDate;
};

#endif // TIMESPAN_H
