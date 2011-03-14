#include "binarymaterial.h"

Spectrum BinaryMaterial::shade (HitRecord &, Light &, const Intersectable &, int) const
{
  return color;
}

BinaryMaterial::BinaryMaterial(Spectrum color) : color(color)
{

}
