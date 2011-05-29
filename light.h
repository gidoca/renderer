#ifndef LIGHT_H
#define LIGHT_H

#include "sampler.h"

class HitRecord;
class Sample;
class Ray;
class Intersectable;
class Spectrum;

class Light
{

  public:
    virtual Spectrum getIntensity(HitRecord & hit, QVector3D & direction, const Intersectable & scene, Sample sample) const = 0;
    virtual Ray getRandomRay(const Sample& sample, double &pdf) const = 0;
};

#endif
