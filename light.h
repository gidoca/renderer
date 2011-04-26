#ifndef LIGHT_H
#define LIGHT_H

#include "spectrum.h"
#include "intersectable.h"
#include "sampler.h"

#include <QtCore/QPointF>

class HitRecord;

class Light
{

  public:
    virtual Spectrum getIntensity(HitRecord & hit, QVector3D & direction, const Intersectable & scene, Sample sample) const = 0;
};

#endif
