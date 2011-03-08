#ifndef MATERIAL_H
#define MATERIAL_H

#include "spectrum.h"
#include "light.h"
class HitRecord;

class Material
{
  public:
    virtual Spectrum shade(HitRecord & hit, Light & light) const = 0;
};

#endif
