#include <values.h>
#include <QDebug>

Values::Values()
{
}

Values::Values(QString eastingX, QString nortingY)
{
    this->eastingX = eastingX;
    this->nortingY = nortingY;
}

QString Values::getEastingX()
{
    return this->eastingX;
}

QString Values::getNortingY()
{
    return this->nortingY;
}

QString Values::toQString()
{
    return QString("\nEastings_X: " + this->eastingX +
                   "\nNorthings_Y: " + this->nortingY);
}
