#include "pointlight.h"

#include "hitrecord.h"
#include "intersectable.h"

Spectrum PointLight::getIntensity (HitRecord & hit, QVector3D & direction, const Intersectable & scene, Sample) const
{
  direction = getDirection(hit.getIntersectingPoint());
  if(isOccluded(hit.getIntersectingPoint(), scene))
  {
    return Spectrum();
  }
  else
  {
    return intensity / direction.lengthSquared();
  }
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
