#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "diffusematerial.h"
#include "intersectable.h"

class PhongMaterial : public DiffuseMaterial
{
  public:
    PhongMaterial(Spectrum color, Spectrum specularColor, double specularCoefficient);

    Spectrum shade(HitRecord& hit, const Light& light) const;

  private:
    const Spectrum color, specularColor;
    const float specularCoefficient;
};

#endif
