#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "material.h"


class MirrorMaterial : public Material
{

public:
  MirrorMaterial(double coefficient);
  Spectrum shade(HitRecord& hit, Light& light, const Intersectable& scene, int depth) const;
  
private:
  double coefficient;
};

#endif // MIRRORMATERIAL_H
