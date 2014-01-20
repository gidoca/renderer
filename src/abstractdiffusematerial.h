#ifndef ABSTRACTDIFFUSEMATERIAL_H
#define ABSTRACTDIFFUSEMATERIAL_H

#include "material.h"

class AbstractDiffuseMaterial : public Material
{
public:
    QVector3D outDirection(const HitRecord & hit, Sample s, float &pdf, cv::Vec3f& brdf) const;


};

#endif // ABSTRACTDIFFUSEMATERIAL_H
