#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

#include "global.h"

class Light
{

  public:
    virtual Spectrum getIntensity(const QVector3D & at, QVector3D & direction, const Intersectable & scene, const Sample& sample) const = 0;
    virtual Ray getRandomRay(const Sample& sample1, const Sample &sample2, float &pdf) const = 0;
};

#endif
