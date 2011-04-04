#ifndef MATERIAL_H
#define MATERIAL_H

#include "spectrum.h"

class HitRecord;
class Light;
class Intersectable;

class Material
{
  public:
    virtual Spectrum shade(HitRecord & hit, QVector3D direction) const = 0;
    virtual bool isMirror() const = 0;
};

#endif
