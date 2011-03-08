#ifndef LIGHT_H
#define LIGHT_H

#define EPSILON 0.00001

#include "spectrum.h"
#include "intersectable.h"

//class Intersectable;

class Light
{

  public:
    virtual QVector3D getDirection(QVector3D location) const = 0;
    virtual Spectrum getIntensity(QVector3D location) const = 0;
    virtual bool isOccluded(QVector3D location, const Intersectable & scene) const = 0;
};

#endif
