#include "environmentmap.h"

#include <cmath>
#include <QVector2D>

EnvironmentMap::EnvironmentMap(const char *filename, float coefficient) : coefficient(coefficient)
{
  HDRLoader loader;
  loader.load(filename, data);
}

EnvironmentMap::~EnvironmentMap()
{
  delete data.cols;
}

Spectrum EnvironmentMap::getIntensity(HitRecord &hit, QVector3D &direction, const Intersectable &scene, QPointF sample) const
{
  QVector3D normalizedDirection = QVector3D(cos(2 * M_PI * sample.y()) * sqrt(sample.x()), sin(2 * M_PI * sample.y()) * sqrt(sample.x()), sqrt(1 - sample.x()));
  normalizedDirection.normalize();
  double theta = acos(QVector3D::dotProduct(normalizedDirection.normalized(), hit.getSurfaceNormal().normalized()));
  QVector3D k = QVector3D::crossProduct(normalizedDirection.normalized(), hit.getSurfaceNormal().normalized());
  direction = normalizedDirection * cos(theta) + QVector3D::crossProduct(k, normalizedDirection) * sin(theta) + k * QVector3D::dotProduct(k, normalizedDirection) * (1 - cos(theta));
  //direction /= direction.length();
  Ray shadowRay(hit.getIntersectingPoint(), direction);
  HitRecord shadowHit = scene.intersect(shadowRay);
  if(shadowHit.intersects())
  {
    return Spectrum();
  }
  else
  {
    QVector2D imageCoords = direction.toVector2D();
    imageCoords *= acos(direction.z()) / (imageCoords.length() * M_PI);
    float x = (((imageCoords.x() + 1.0) / 2.0) * data.width - 1);
    float y = (((imageCoords.y() + 1.0) / 2.0) * data.height - 1);
    long index = 3 * ((int)x + data.width * (int)y);
    return coefficient * M_PI / atan(sqrt(sample.x() / (1 - sample.x()))) * Spectrum(data.cols[index], data.cols[index + 1],data.cols[index + 2]);
  }
}
