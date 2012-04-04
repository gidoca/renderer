#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "global.h"
#include "material.h"

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(Spectrum color) : color(color) {}
    bool isMirror() const;
    Spectrum shade(const HitRecord& hit, QVector3D direction) const;
protected:
    const Spectrum color;
};

#endif // DIFFUSEMATERIAL_H
