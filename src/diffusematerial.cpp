#include "diffusematerial.h"

#include <cmath>

bool DiffuseMaterial::isMirror() const
{
  return false;
}

Spectrum DiffuseMaterial::shade(const HitRecord&, QVector3D) const
{
  return color / M_PI;
}
