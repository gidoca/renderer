#include "binarymaterial.h"

Spectrum BinaryMaterial::shade (HitRecord &, QVector3D) const
{
  return color;
}

BinaryMaterial::BinaryMaterial(Spectrum color) : color(color)
{

}
