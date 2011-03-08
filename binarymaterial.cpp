#include "binarymaterial.h"

Spectrum BinaryMaterial::shade (HitRecord & hit, Light & light) const
{
  return color;
}

BinaryMaterial::BinaryMaterial(Spectrum color) : color(color)
{

}
