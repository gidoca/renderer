#include "arealight.h"

#include "path.h"

#include <cassert>
#include <cmath>

AreaLight::AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, Spectrum intensity) : origin(origin), uDirection(uDirection), vDirection(vDirection), intensity(intensity), normal(QVector3D::crossProduct(uDirection, vDirection))
{
}

Spectrum AreaLight::getIntensity(HitRecord & hit, QVector3D &direction, const Intersectable &scene, Sample sample) const
{
  QPointF p = sample.getSample();
  QVector3D lightLocation = getLocation(p);
  direction = hit.getIntersectingPoint() - lightLocation;
  HitRecord shadowHit = scene.intersect(Ray(hit.getIntersectingPoint(), -direction.normalized()), EPSILON, direction.length());
  if(shadowHit.intersects())
  {
    return Spectrum();
  }
  else
  {
    double c = QVector3D::dotProduct(direction.normalized(), normal.normalized()) / direction.lengthSquared() * normal.length();
    assert(!isnan(c));
    if(c < 0) c = 0;
    return c * intensity;
  }
}

Ray AreaLight::getRandomRay(const Sample &sample, double &pdf) const
{
  QVector3D location = getLocation(sample.getSample());
  QVector3D direction = sample.getCosineWeightedDirection(normal, pdf);
  return Ray(location, direction);
}

QVector3D AreaLight::getLocation(QPointF p) const
{
  return origin + p.x() * uDirection + p.y() * vDirection;
}
