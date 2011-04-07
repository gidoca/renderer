#include "arealight.h"

AreaLight::AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, Spectrum intensity) : origin(origin), uDirection(uDirection), vDirection(vDirection), intensity(intensity), normal(QVector3D::crossProduct(uDirection, vDirection))
{
}

Spectrum AreaLight::getIntensity(HitRecord & hit, QVector3D &direction, const Intersectable &scene, QPointF sample) const
{
  QVector3D lightLocation = origin + sample.x() * uDirection + sample.y() * vDirection;
  direction = hit.getIntersectingPoint() - lightLocation;
  HitRecord shadowHit = scene.intersect(Ray(hit.getIntersectingPoint(), -direction.normalized()), EPSILON, direction.length());
  if(shadowHit.intersects())
  {
    return Spectrum();
  }
  else
  {
    double c = QVector3D::dotProduct(direction.normalized(), normal.normalized()) / direction.lengthSquared() * normal.length();
    if(c < 0) c = 0;
    return c * intensity;
  }
}
