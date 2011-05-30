#include "conelight.h"

#include "hitrecord.h"
#include "intersectable.h"

#include <cmath>

Spectrum ConeLight::getIntensity(HitRecord &hit, QVector3D &direction, const Intersectable &scene, Sample sample) const
{
  direction = hit.getIntersectingPoint() - location;
  HitRecord shadowHit = scene.intersect(Ray(hit.getIntersectingPoint(), -direction.normalized()), EPSILON, direction.length());
  Spectrum part = Spectrum(1, 1, 1);
  if(shadowHit.intersects() && shadowHit.getMaterial().isParticipating())
  {
    shadowHit = scene.intersect(Ray(shadowHit.getIntersectingPoint(), -direction.normalized()), EPSILON, (shadowHit.getIntersectingPoint() - location).length());
    part *= (shadowHit.getIntersectingPoint() - location).length() * .001;
  }
  double rayAngle = QVector3D::dotProduct(direction.normalized(), this->direction.normalized());
  if(0 <= acos(rayAngle) && acos(rayAngle) <= openingAngle && !shadowHit.intersects())
  {
    return intensity * part / direction.lengthSquared();
  }
  else
  {
    return Spectrum();
  }
}

Ray ConeLight::getRandomRay(const Sample &sample, double &pdf) const
{
  return Ray(location, sample.getCosineWeightedDirection(direction, pdf, openingAngle));
}
