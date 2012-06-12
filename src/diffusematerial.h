#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "global.h"
#include "material.h"

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(cv::Vec3f color) : color(color) {}
    bool isMirror() const;
    cv::Vec3f shade(const HitRecord& hit, QVector3D direction) const;
protected:
    const cv::Vec3f color;
};

#endif // DIFFUSEMATERIAL_H
