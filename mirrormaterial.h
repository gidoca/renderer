#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "material.h"


class MirrorMaterial : public Material
{

public:
  MirrorMaterial(float coefficient): coefficient(coefficient)
  {

  }

  Spectrum shade(HitRecord& hit, QVector3D direction) const;
  bool isMirror() const;
  bool isParticipating() const;
  
private:
  float coefficient;
};

#endif // MIRRORMATERIAL_H
