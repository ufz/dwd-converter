#ifndef STATION_H
#define STATION_H

#include <QString>
#include <values.h>
#include <magnitudes.h>
#include <timespan.h>
#include <QStringList>

class Station
{
public:
    Station();
    Station(QStringList splittedTextLine);

    QString getID();
    TimeSpan* getTimeSpanPressure();
    TimeSpan* getTimeSpanRelativeHumidity();
    TimeSpan* getTimeSpanAverageTemperature();
    TimeSpan* getTimeSpanMaxTemperature();
    TimeSpan* getTimeSpanMinTemperature();
    QList<QString>* getPressure();
    QList<QString>* getRelativeHumidity();
    QList<QString>* getAverageTemperature();
    QList<QString>* getMaxTemperature();
    QList<QString>* getMinTemperature();

    QDate getMinStartDate();
    QDate getMaxEndDate();
    qint32 getMaxDayDif();

    void setID(QString id);

    void setPressure(QStringList values);
    void setRelativeHumidity(QStringList values);
    void setAverageTemperature(QStringList values);
    void setMaxTemperature(QStringList values);
    void setMinTemperature(QStringList values);

    void setTimeSpanPressure(QString startDate, QString endDate);
    void setTimeSpanRelativeHumidity(QString startDate, QString endDate);
    void setTimeSpanAverageTemperature(QString startDate, QString endDate);
    void setTimeSpanMaxTemperature(QString startDate, QString endDate);
    void setTimeSpanMinTemperature(QString startDate, QString endDate);

    bool isConsistent();
    void writeFile(QString path);
    void generateMissingValues();
    void generateMissingMeasureTypes();

    QString toQString();

private:
    QString         ID;
    QString         stationName;
    Values          values;
    Magnitudes      magnitudes;
    QString         blID;                   //TODO was ist BL_ID?
    TimeSpan        timeSpanPressure;
    TimeSpan        timeSpanRelativeHumidity;
    TimeSpan        timeSpanAverageTemperature;
    TimeSpan        timeSpanMaxTemperature;
    TimeSpan        timeSpanMinTemperature;
    QString         state;

    QList<QString>  pressure;
    QList<QString>  relativeHumidity;
    QList<QString>  averageTemperature;
    QList<QString>  maxTemperature;
    QList<QString>  minTemperature;
};

#endif // STATION_H
