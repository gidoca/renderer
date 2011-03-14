#ifndef MATERIAL_H
#define MATERIAL_H

#include "spectrum.h"

class HitRecord;
class Light;
class Intersectable;

class Material
{
  public:
    virtual Spectrum shade(HitRecord & hit, Light & light, const Intersectable & scene) const = 0;
};

#endif
