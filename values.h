#ifndef VALUES_H
#define VALUES_H

#include <QString>

class Values
{
public:
    Values();
    Values(QString eastingX, QString nortingY);

    QString getEastingX();
    QString getNortingY();

    QString toQString();

private:
    QString eastingX;
    QString nortingY;
};

#endif // VALUES_H
