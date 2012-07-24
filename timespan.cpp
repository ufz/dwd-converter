#include <timespan.h>
#include <QRegExp>
#include <QDebug>
#include <QStringList>

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

void TimeSpan::setStartDate(QDate date)
{
    this->startDate = date;
}

void TimeSpan::setStartDate(QString date) {
    QStringList dateValues = date.split(QRegExp("\\D+"));
    this->startDate = QDate(dateValues[0].toInt(),
                            dateValues[1].toInt(),
                            dateValues[2].toInt());
}

void TimeSpan::setEndDate(QDate date)
{
    this->endDate = date;
}

void TimeSpan::setEndDate(QString date) {
    QStringList dateValues = date.split(QRegExp("\\D+"));
    this->endDate = QDate(dateValues[0].toInt(),
                          dateValues[1].toInt(),
                          dateValues[2].toInt());
}
