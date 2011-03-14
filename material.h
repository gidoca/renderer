#ifndef MATERIAL_H
#define MATERIAL_H

#define MAX_NUM_BOUNCES 6

#include "spectrum.h"

class HitRecord;
class Light;
class Intersectable;

class Material
{
  public:
    virtual Spectrum shade(HitRecord & hit, Light & light, const Intersectable & scene, int depth) const = 0;
};

#endif
