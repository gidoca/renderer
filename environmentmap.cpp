#include "environmentmap.h"

#include <cmath>
#include <QVector2D>

EnvironmentMap::EnvironmentMap(const char *filename)
{
  HDRLoader loader;
  loader.load(filename, data);
}

Spectrum EnvironmentMap::getIntensity(HitRecord &hit, QVector3D &direction, const Intersectable &scene, QPointF sample) const
{
  direction = QVector3D(cos(2 * M_PI * sample.y()) * sqrt(sample.x()), sin(2 * M_PI * sample.y()) * sqrt(sample.x()), sqrt(1 - sample.x()));
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
    int x = (int) (((imageCoords.x() + 1) / 2) * data.width - 1);
    int y = (int) (((imageCoords.y() + 1) / 2) * data.height - 1);
    int index = 3 * (x + data.width * y);
    return 20 * Spectrum(data.cols[index], data.cols[index + 1],data.cols[index + 2]);
  }
}
