#ifndef LIGHT_H
#define LIGHT_H

#include "spectrum.h"

class Light
{

  public:
    virtual QVector3D getDirection(QVector3D location) const = 0;
    virtual Spectrum getIntensity(QVector3D location) const = 0;

};

#endif
