#ifndef DARKMATTER_H
#define DARKMATTER_H

#include "diffusematerial.h"

class DarkMatter : public DiffuseMaterial
{
  public:
    Spectrum shade(HitRecord&, QVector3D direction) const;
};

#endif
