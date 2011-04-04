#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "material.h"


class MirrorMaterial : public Material
{

public:
  MirrorMaterial(double coefficient);
  Spectrum shade(HitRecord& hit, const Light& light) const;
  bool isMirror() const;
  
private:
  double coefficient;
};

#endif // MIRRORMATERIAL_H
