#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "global.h"
#include "diffusematerial.h"
#include "intersectable.h"

class PhongMaterial : public DiffuseMaterial
{
  public:
    PhongMaterial(cv::Vec3f color, cv::Vec3f specularColor, float specularCoefficient) : DiffuseMaterial(color), specularColor(specularColor), specularCoefficient(specularCoefficient)
    {

    }

    cv::Vec3f shade(const HitRecord& hit, QVector3D direction) const;

  private:
    const cv::Vec3f specularColor;
    const float specularCoefficient;
};

#endif
