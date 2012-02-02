#include "binarymaterial.h"

Spectrum BinaryMaterial::shade (const HitRecord &, QVector3D) const
{
  return color;
}

