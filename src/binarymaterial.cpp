#include "binarymaterial.h"

Spectrum BinaryMaterial::shade (HitRecord &, QVector3D) const
{
  return color;
}

