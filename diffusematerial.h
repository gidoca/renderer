#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "material.h"

class DiffuseMaterial : public Material
{
  public:
    DiffuseMaterial(Spectrum color);

    Spectrum shade(QVector3D location, QVector3D normal) const;

  private:
    const Spectrum color;
};

#endif
