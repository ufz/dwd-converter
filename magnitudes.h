#ifndef MAGNITUDES_H
#define MAGNITUDES_H

#include <QString>

class Magnitudes
{
public:
    Magnitudes();
    Magnitudes(QString altitude, QString longtitude, QString latitude);

    QString getAltitude();
    QString getLongitude();
    QString getLatitude();

    QString toQString();

private:
    QString altitude;
    QString longitude;
    QString latitude;
};

#endif // MAGNITUDES_H
