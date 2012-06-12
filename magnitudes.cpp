#include <magnitudes.h>
#include <QDebug>

Magnitudes::Magnitudes()
{
}

Magnitudes::Magnitudes(QString altitude, QString longtitude, QString latitude)
{
    this->altitude = altitude;
    this->longitude = longtitude;
    this->latitude = latitude;
}

QString Magnitudes::getAltitude()
{
    return this->altitude;
}

QString Magnitudes::getLongitude()
{
    return this->longitude;
}

QString Magnitudes::getLatitude()
{
    return this->latitude;
}

QString Magnitudes::toQString()
{
    return QString( "\nAltitude: " + this->altitude +
                    "\nLongtitude: " + this->longitude +
                    "\nLatitude: " + this->latitude);
}
