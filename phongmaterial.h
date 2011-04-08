#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "diffusematerial.h"
#include "intersectable.h"

class PhongMaterial : public DiffuseMaterial
{
  public:
    PhongMaterial(Spectrum color, Spectrum specularColor, double specularCoefficient) : color(color), specularColor(specularColor), specularCoefficient(specularCoefficient)
    {

    }


    Spectrum shade(HitRecord& hit, QVector3D direction) const;

  private:
    const Spectrum color, specularColor;
    const float specularCoefficient;
};

#endif
