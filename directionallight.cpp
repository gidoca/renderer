#include "directionallight.h"
#include "hitrecord.h"
#include "ray.h"
#include "intersectable.h"

#include <iostream>

Spectrum DirectionalLight::getDirection() const
{
  return direction;
}

Spectrum DirectionalLight::getIntensity(HitRecord & hit, QVector3D & direction, const Intersectable & scene, QPointF) const
{
  direction = getDirection();
  if(isOccluded(hit.getIntersectingPoint(), scene))
  {
    return Spectrum();
  }
  else
  {
    return intensity;
  }
}

bool DirectionalLight::isOccluded(QVector3D location, const Intersectable & scene) const
{
  HitRecord hit = scene.intersect(Ray(location, -direction.normalized()));
  return hit.intersects();
}
