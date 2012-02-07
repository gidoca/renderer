#include "conelight.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "sampler.h"

#include <cmath>

Spectrum ConeLight::getIntensity(const QVector3D & at, QVector3D &direction, const Intersectable &scene, const Sample &) const
{
  direction = at - location;
  HitRecord shadowHit = scene.intersect(Ray(at, -direction.normalized()), EPSILON, direction.length());
  Spectrum part = Spectrum(1, 1, 1);
  if(shadowHit.intersects() && shadowHit.getMaterial().isParticipating())
  {
    shadowHit = scene.intersect(Ray(shadowHit.getIntersectingPoint(), -direction.normalized()), EPSILON, (shadowHit.getIntersectingPoint() - location).length());
    part *= (shadowHit.getIntersectingPoint() - location).length() * .001;
  }
  float rayAngle = QVector3D::dotProduct(direction.normalized(), this->direction.normalized());
  if(0 <= acos(rayAngle) && acos(rayAngle) <= openingAngle && !shadowHit.intersects())
  {
    return intensity * part / direction.lengthSquared();
  }
  else
  {
    return Spectrum();
  }
}

Ray ConeLight::getRandomRay(const Sample &sample, const Sample&, float &pdf) const
{
  return Ray(location, sample.getCosineWeightedDirection(direction, pdf, openingAngle));
}
