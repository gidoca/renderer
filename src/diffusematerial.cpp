#include "diffusematerial.h"

#include <cmath>

bool DiffuseMaterial::isMirror() const
{
  return false;
}

cv::Vec3f DiffuseMaterial::shade(const HitRecord&, QVector3D) const
{
  return color * (1 / M_PI);
}
