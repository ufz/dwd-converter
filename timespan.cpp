#include <timespan.h>
#include <QDebug>

TimeSpan::TimeSpan()
{
}

TimeSpan::TimeSpan(QDate startDate, QDate endDate)
{
    this->startDate = startDate;
    this->endDate = endDate;
}

QDate TimeSpan::getStartDate()
{
    return this->startDate;
}

QDate TimeSpan::getEndDate()
{
    return this->endDate;
}

QString TimeSpan::toQString()
{
    return QString("\nStartDate: " + this->startDate.toString("yyyy-MM-dd") +
                   "\nEndDate: " + this->endDate.toString("yyyy-MM-dd"));
}

/*
  Returns true if start date or end date are empty
*/
bool TimeSpan::isEmpty()
{
    if(this->startDate.isNull() || this->endDate.isNull())
        return true;
    else
        return false;
}

QDate TimeSpan::setStartDate(QDate date)
{
    this->startDate = date;
}

QDate TimeSpan::setEndDate(QDate date)
{
    this->endDate = date;
}
