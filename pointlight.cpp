#include "pointlight.h"

PointLight::PointLight(QVector3D location, Spectrum intensity) : location(location), intensity(intensity)
{
  
}

Spectrum PointLight::getIntensity (QVector3D at) const
{
  return intensity / getDirection(at).lengthSquared();
}

QVector3D PointLight::getDirection (QVector3D at) const
{
  return at - location;
}

bool PointLight::isOccluded(QVector3D location, const Intersectable & scene) const
{
  return false;
}
