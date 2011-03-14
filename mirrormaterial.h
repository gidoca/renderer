#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "material.h"


class MirrorMaterial : public Material
{

public:
    Spectrum shade(HitRecord& hit, Light& light, const Intersectable& scene, int depth) const;
};

#endif // MIRRORMATERIAL_H
