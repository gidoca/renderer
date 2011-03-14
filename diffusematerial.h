#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "material.h"
#include "intersectable.h"

class DiffuseMaterial : public Material
{
  public:
    DiffuseMaterial(Spectrum color, Spectrum specularColor, double specularCoefficient);

    Spectrum shade(HitRecord& hit, Light& light, const Intersectable& scene, int depth) const;

  private:
    const Spectrum color, specularColor;
    const float specularCoefficient;
};

#endif
