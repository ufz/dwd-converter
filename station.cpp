#include <station.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>

QDate generateQDateFromQString(QString dateString);

Station::Station()
{
}

Station::Station(QStringList textLine)
{
    /*
        splittedTextLine structure as in the data file:
        Stat_ID, Easting_x, Norting_Y, Altitude, Longitude, Latitude, Bl_ID,
        State, Station_Name
    */

    //Fill ID with missing zeros
    if(textLine.at(0).size() < 5)
    {
        this->ID.fill('0', 5-textLine.at(0).size());
        this->ID.append(textLine.at(0));
    }else
        this->ID                        = textLine.at(0);

    this->values                        = Values(textLine.at(1),
                                                 textLine.at(2));
    this->magnitudes                    = Magnitudes(textLine.at(3),
                                                     textLine.at(4),
                                         textLine.at(5));
    this->blID                          = textLine.at(6);
    this->state                         = textLine.at(7);
    this->stationName                   = textLine.at(8);
}

QString Station::getID()
{
    return this->ID;
}

/*
  Returns informations of the station
*/
QString Station::toQString()
{
    return QString("\nStat_ID: " + this->ID +
                   "\nState: " + this->state +
                   "\nStation_Name: " + this->stationName +
                   this->values.toQString() +
                   this->magnitudes.toQString() +
                   "\nBl_ID: " + this->blID +
                   "\nTime span pressure:" +
                   this->timeSpanPressure.toQString() +
                   "\nTime span relative humidity:" +
                   this->timeSpanRelativeHumidity.toQString() +
                   "\nTime span average temperature" +
                   this->timeSpanAverageTemperature.toQString() +
                   "\nTime span max temperature" +
                   this->timeSpanMaxTemperature.toQString() +
                   "\nTime span min temperature" +
                   this->timeSpanMinTemperature.toQString() +
                   "\nPressure size:" +
                   QString().setNum(this->pressure.size()) +
                   "\nRelative humidity size:" +
                   QString().setNum(this->relativeHumidity.size()) +
                   "\nAverage temperature:" +
                   QString().setNum(this->averageTemperature.size()) +
                   "\nMax temperature:" +
                   QString().setNum(this->maxTemperature.size()) +
                   "\nMin temperature:" +
                   QString().setNum(this->minTemperature.size()) +
                   "\n");
}

TimeSpan* Station::getTimeSpanPressure()
{
    return &this->timeSpanPressure;
}

TimeSpan* Station::getTimeSpanRelativeHumidity()
{
    return &this->timeSpanRelativeHumidity;
}

TimeSpan* Station::getTimeSpanAverageTemperature()
{
    return &this->timeSpanAverageTemperature;
}

TimeSpan* Station::getTimeSpanMaxTemperature()
{
    return &this->timeSpanMaxTemperature;
}

TimeSpan* Station::getTimeSpanMinTemperature()
{
    return &this->timeSpanMinTemperature;
}

/*
  Returns difference between minStartDate and maxEndDate
*/
qint32 Station::getMaxDayDif()
{
    return this->getMinStartDate().daysTo(this->getMaxEndDate()) + 1;
}

void Station::setTimeSpanPressure(QString startDate, QString endDate)
{
    this->timeSpanPressure = TimeSpan(generateQDateFromQString(startDate),
                           generateQDateFromQString(endDate));
}

void Station::setTimeSpanRelativeHumidity(QString startDate, QString endDate)
{
    this->timeSpanRelativeHumidity = TimeSpan(generateQDateFromQString(startDate),
                              generateQDateFromQString(endDate));
}

void Station::setTimeSpanAverageTemperature(QString startDate, QString endDate)
{
    this->timeSpanAverageTemperature = TimeSpan(generateQDateFromQString(startDate),
                              generateQDateFromQString(endDate));
}

void Station::setTimeSpanMaxTemperature(QString startDate, QString endDate)
{
    this->timeSpanMaxTemperature = TimeSpan(generateQDateFromQString(startDate),
                              generateQDateFromQString(endDate));
}

void Station::setTimeSpanMinTemperature(QString startDate, QString endDate)
{
    this->timeSpanMinTemperature = TimeSpan(generateQDateFromQString(startDate),
                              generateQDateFromQString(endDate));
}

QDate generateQDateFromQString(QString dateString)
{
    QStringList dateValues   = dateString.split(QRegExp("\\D+"));
    return QDate(dateValues[0].toInt(),
                 dateValues[1].toInt(),
                 dateValues[2].toInt());
}

QList<QString>* Station::getPressure()
{
    return &this->pressure;
}

QList<QString>* Station::getRelativeHumidity()
{
    return &this->relativeHumidity;
}

QList<QString>* Station::getAverageTemperature()
{
    return &this->averageTemperature;
}

QList<QString>* Station::getMaxTemperature()
{
    return &this->maxTemperature;
}

QList<QString>* Station::getMinTemperature()
{
    return &this->minTemperature;
}

/*
  Returns the earlies date on wihich a value was measured
*/
QDate Station::getMinStartDate()
{
    QDate date;

    if(!this->timeSpanPressure.getStartDate().isNull())
        date = this->timeSpanPressure.getStartDate();

    else if (!this->timeSpanRelativeHumidity.getStartDate().isNull())
        date = this->timeSpanRelativeHumidity.getStartDate();

    else if (!this->timeSpanAverageTemperature.getStartDate().isNull())
        date = this->timeSpanAverageTemperature.getStartDate();

    else if(!this->timeSpanMaxTemperature.getStartDate().isNull())
        date = this->timeSpanMaxTemperature.getStartDate();

    else
        date = this->timeSpanMinTemperature.getStartDate();

    if(this->timeSpanPressure.getStartDate().operator <(date) &&
            !this->timeSpanPressure.getStartDate().isNull())
        date = this->timeSpanPressure.getStartDate();

    if(this->timeSpanRelativeHumidity.getStartDate().operator <(date) &&
            !this->timeSpanRelativeHumidity.getStartDate().isNull())
        date = this->timeSpanRelativeHumidity.getStartDate();

    if(this->timeSpanAverageTemperature.getStartDate().operator <(date) &&
            !this->timeSpanAverageTemperature.getStartDate().isNull())
        date = this->timeSpanAverageTemperature.getStartDate();

    if(this->timeSpanMaxTemperature.getStartDate().operator <(date) &&
            !this->timeSpanMaxTemperature.getStartDate().isNull())
        date = this->timeSpanMaxTemperature.getStartDate();

    if(this->timeSpanMinTemperature.getStartDate().operator <(date) &&
            !this->timeSpanMinTemperature.getStartDate().isNull())
        date = this->timeSpanMinTemperature.getStartDate();

    return date;
}

/*
  Returns the latest date on which a value was measured
*/
QDate Station::getMaxEndDate()
{
    QDate date;

    if(!this->timeSpanPressure.getEndDate().isNull())
        date = this->timeSpanPressure.getEndDate();

    else if (!this->timeSpanRelativeHumidity.getEndDate().isNull())
        date = this->timeSpanRelativeHumidity.getEndDate();

    else if (!this->timeSpanAverageTemperature.getEndDate().isNull())
        date = this->timeSpanAverageTemperature.getEndDate();

    else if(!this->timeSpanMaxTemperature.getEndDate().isNull())
        date = this->timeSpanMaxTemperature.getEndDate();

    else
        date = this->timeSpanMinTemperature.getEndDate();


    if(this->timeSpanPressure.getEndDate().operator >(date) &&
            !this->timeSpanPressure.getEndDate().isNull())
        date = this->timeSpanPressure.getEndDate();

    if(this->timeSpanRelativeHumidity.getEndDate().operator >(date) &&
            !this->timeSpanRelativeHumidity.getEndDate().isNull())
        date = this->timeSpanRelativeHumidity.getEndDate();

    if(this->timeSpanAverageTemperature.getEndDate().operator >(date) &&
            !this->timeSpanAverageTemperature.getEndDate().isNull())
        date = this->timeSpanAverageTemperature.getEndDate();

    if(this->timeSpanMaxTemperature.getEndDate().operator >(date) &&
            !this->timeSpanMaxTemperature.getEndDate().isNull())
        date = this->timeSpanMaxTemperature.getEndDate();

    if(this->timeSpanMinTemperature.getEndDate().operator >(date) &&
            !this->timeSpanMinTemperature.getEndDate().isNull())
        date = this->timeSpanMinTemperature.getEndDate();

    return date;
}

/*
 Checks if this station has any inconsistencies
*/
bool Station::isConsistent()
{
    //Check if the timespan of a measure type is equal to the amount of values
    if(!this->timeSpanPressure.isEmpty())
    {
        if((this->timeSpanPressure.getStartDate().daysTo(this->timeSpanPressure.getEndDate()) + 1) != this->pressure.size())
        {
            return false;
        }
    }else if(!this->timeSpanRelativeHumidity.isEmpty())
    {
         if((this->timeSpanRelativeHumidity.getStartDate().daysTo(this->timeSpanRelativeHumidity.getEndDate()) + 1) != this->relativeHumidity.size())
            return false;
    }else if(!this->timeSpanAverageTemperature.isEmpty())
    {
         if((this->timeSpanAverageTemperature.getStartDate().daysTo(this->timeSpanAverageTemperature.getEndDate()) + 1) != this->averageTemperature.size())
            return false;
    }else if (!this->timeSpanMaxTemperature.isEmpty())
    {
        if((this->timeSpanMaxTemperature.getStartDate().daysTo(this->timeSpanMaxTemperature.getEndDate()) + 1) != this->maxTemperature.size())
            return false;
    }else if(!this->timeSpanMinTemperature.isEmpty())
    {
        if((this->timeSpanMinTemperature.getStartDate().daysTo(this->timeSpanMinTemperature.getEndDate()) + 1) != this->minTemperature.size())
            return false;
    }

    return true;
}

/*
  Write station to file with the given path
*/
void Station::writeFile(QString path)
{
    QDate date;

    QFile outputFile(path + this->ID + ".csv");
    outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream tsOut(&outputFile);

    if(outputFile.exists())
    {
        qDebug() << "Writing data for station"
                 << this->ID
                 << "to" << outputFile.fileName();

        //Write header
        tsOut << "DWDI_"    << this->ID << "\n";
        tsOut << "Nr"       << ","
              << "date"     << ","
              << "pre"      << ","
              << "rh"       << ","
              << "tem"      << ","
              << "tmax"     << ","
              << "tmin"     << "\n";

        //Write data
        date = this->getMinStartDate();
        for(int i=0; i < this->getMaxDayDif(); i++)
        {
            tsOut << i << ","
                  << date.addDays(i).toString("yyyy-MM-dd")
                  << ","
                  << this->pressure.at(i)
                  << ","
                  << this->relativeHumidity.at(i)
                  << ","
                  << this->averageTemperature.at(i)
                  << ","
                  << this->maxTemperature.at(i)
                  << ","
                  << this->minTemperature.at(i)
                  << "\n";
       }
        outputFile.close();
    }else{
        qDebug() << "Could not create outputfile for" << this->ID << endl;
        exit(1);
    }
}

/*
  Generates values for measure types which are not given between minStartDate and maxEndDate
*/
void Station::generateMissingValues()
{
    qDebug() << "Generate missing values for station" << this->getID();
    quint32 dif;

    //If start and/or end date differs from minStarteDate and/or maxEndDate
    if(this->pressure.size() < this->getMaxDayDif())
    {
        dif = this->getMinStartDate().daysTo(
                    this->timeSpanPressure.getStartDate());
        qDebug() << dif;
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->pressure.prepend("-9");

        dif = this->timeSpanPressure.getEndDate().daysTo(this->getMaxEndDate());
        qDebug() << dif;
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->pressure.append("-9");
    }

    if(this->relativeHumidity.size() < this->getMaxDayDif())
    {
        qDebug() << this->relativeHumidity.size() << "rh";
        dif = this->getMinStartDate().daysTo(
                   this->timeSpanRelativeHumidity.getStartDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
               this->relativeHumidity.prepend("-9");
        dif = this->timeSpanRelativeHumidity.getEndDate().daysTo(this->getMaxEndDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->relativeHumidity.append("-9");
    }

    if(this->averageTemperature.size() < this->getMaxDayDif())
    {
        dif = this->getMinStartDate().daysTo(
                    this->timeSpanAverageTemperature.getStartDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->averageTemperature.prepend("-999");

        dif =this->timeSpanAverageTemperature.getEndDate().daysTo(this->getMaxEndDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->averageTemperature.append("-999");
    }

    if(this->maxTemperature.size() < this->getMaxDayDif())
    {
        dif = this->getMinStartDate().daysTo(
                    this->timeSpanMaxTemperature.getStartDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->maxTemperature.prepend("-999");

        dif = this->timeSpanMaxTemperature.getEndDate().daysTo(this->getMaxEndDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->maxTemperature.append("-999");
    }

    if(this->minTemperature.size() < this->getMaxDayDif())
    {
        dif = this->getMinStartDate().daysTo(
                    this->timeSpanMinTemperature.getStartDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->minTemperature.prepend("-999");

        dif = this->timeSpanMinTemperature.getEndDate().daysTo(this->getMaxEndDate());
        if(dif > 0)
            for(quint32 i = 0; i < dif; i++)
                this->minTemperature.append("-999");
    }
}

/*
  Generates missing values for empty measure types
*/
void Station::generateMissingMeasureTypes()
{
    qDebug() << "Generate missing measure types for station" << this->getID();
    //If the measure type is empty set time span
    //and fill it with dummy values
    if(this->pressure.isEmpty())
        for(int i = 0; i < this->getMaxDayDif(); i++)
            this->pressure.append("-9");

    if(this->relativeHumidity.isEmpty())
        for(int i = 0; i < this->getMaxDayDif(); i++)
            this->relativeHumidity.append("-9");

    if(this->averageTemperature.isEmpty())
        for(int i = 0; i < this->getMaxDayDif(); i++)
           this->averageTemperature.append("-999");

    if(this->maxTemperature.isEmpty())
        for(int i = 0; i < this->getMaxDayDif(); i++)
            this->maxTemperature.append("-999");

    if(this->minTemperature.isEmpty())
        for(int i = 0; i < this->getMaxDayDif(); i++)
            this->minTemperature.append("-999");
}

