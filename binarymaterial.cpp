#include "binarymaterial.h"

Spectrum BinaryMaterial::shade (HitRecord &, const Light &) const
{
  return color;
}

BinaryMaterial::BinaryMaterial(Spectrum color) : color(color)
{

}
