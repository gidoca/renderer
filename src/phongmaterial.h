#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "global.h"
#include "diffusematerial.h"
#include "intersectable.h"

class PhongMaterial : public DiffuseMaterial
{
  public:
    PhongMaterial(Spectrum color, Spectrum specularColor, float specularCoefficient) : color(color), specularColor(specularColor), specularCoefficient(specularCoefficient)
    {

    }


    Spectrum shade(const HitRecord& hit, QVector3D direction) const;

  private:
    const Spectrum color, specularColor;
    const float specularCoefficient;
};

#endif