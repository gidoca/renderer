#include "binarymaterial.h"

Spectrum BinaryMaterial::shade (HitRecord & , Light & , const Intersectable &) const
{
  return color;
}

BinaryMaterial::BinaryMaterial(Spectrum color) : color(color)
{

}
