#ifndef MATERIAL_H
#define MATERIAL_H

#include "spectrum.h"

class HitRecord;
class Light;

class Material
{
  public:
    virtual Spectrum shade(HitRecord & hit, Light & light) const = 0;
};

#endif
