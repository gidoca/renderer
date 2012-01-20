#include "arealight.h"

#include "path.h"
#include "hitrecord.h"
#include "intersectable.h"

#include <cassert>
#include <cmath>
#include <iostream>

AreaLight::AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, Spectrum intensity) : origin(origin), uDirection(uDirection), vDirection(vDirection), intensity(intensity), normal(QVector3D::crossProduct(uDirection, vDirection))
{
}

Spectrum AreaLight::getIntensity(const QVector3D & at, QVector3D &direction, const Intersectable &scene, const Sample &sample) const
{
  QPointF p = sample.getSample();
  QVector3D lightLocation = getLocation(p);
  direction = at - lightLocation;
  HitRecord shadowHit = scene.intersect(Ray(at, -direction.normalized()), EPSILON, direction.length());
  Spectrum part = Spectrum(1, 1, 1);
  while(shadowHit.intersects() && shadowHit.getMaterial().isParticipating())
  {
    shadowHit = scene.intersect(Ray(shadowHit.getIntersectingPoint(), -direction.normalized()), EPSILON, (shadowHit.getIntersectingPoint() - lightLocation).length());
    part *= (shadowHit.getIntersectingPoint() - lightLocation).length() * .001;
  }
  if(shadowHit.intersects())
  {
    return Spectrum();
  }
  else
  {
    float c = QVector3D::dotProduct(direction.normalized(), normal.normalized()) / direction.lengthSquared() * normal.length();
    assert(!isnan(c));
    if(c < 0) c = 0;
    return c * intensity * part;
  }
}

Ray AreaLight::getRandomRay(const Sample &sample1, const Sample &sample2, float &pdf) const
{
  QVector3D location = getLocation(sample1.getSample());
  QVector3D direction = sample2.getCosineWeightedDirection(normal, pdf);
  return Ray(location, direction);
}

QVector3D AreaLight::getLocation(QPointF p) const
{
  return origin + p.x() * uDirection + p.y() * vDirection;
}
