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
  QVector3D direction = getDirection(location);
  HitRecord hit = scene.intersect(Ray(location, -direction.normalized()), EPSILON, direction.length());
  return hit.intersects();
}
