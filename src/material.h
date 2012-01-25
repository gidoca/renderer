#ifndef MATERIAL_H
#define MATERIAL_H

#include "global.h"
#include "spectrum.h"

class Material
{
  public:
    virtual Spectrum shade(HitRecord & hit, QVector3D direction) const = 0;
    virtual bool isMirror() const = 0;
    virtual bool isParticipating() const = 0;
};

#endif
