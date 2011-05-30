#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "material.h"


class MirrorMaterial : public Material
{

public:
  MirrorMaterial(double coefficient): coefficient(coefficient)
  {

  }

  Spectrum shade(HitRecord& hit, QVector3D direction) const;
  bool isMirror() const;
  bool isParticipating() const;
  
private:
  double coefficient;
};

#endif // MIRRORMATERIAL_H
