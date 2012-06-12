#ifndef MIRRORMATERIAL_H
#define MIRRORMATERIAL_H

#include "global.h"
#include "material.h"

class MirrorMaterial : public Material
{

public:
  MirrorMaterial(float coefficient): coefficient(coefficient)
  {

  }

  cv::Vec3f shade(const HitRecord& hit, QVector3D direction) const;
  bool isMirror() const;
  
private:
  float coefficient;
};

#endif // MIRRORMATERIAL_H
