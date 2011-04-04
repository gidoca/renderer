#ifndef LIGHT_H
#define LIGHT_H

#include "spectrum.h"
#include "intersectable.h"

//class Intersectable;

class Light
{

  public:
    virtual Spectrum getIntensity(QVector3D location, QVector3D & direction) const = 0;
    virtual bool isOccluded(QVector3D location, const Intersectable & scene) const = 0;
};

#endif
