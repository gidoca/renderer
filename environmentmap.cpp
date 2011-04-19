#include "environmentmap.h"

#include <cmath>
#include <QVector2D>

EnvironmentMap::EnvironmentMap(const char *filename, float coefficient) : coefficient(coefficient)
{
  HDRLoader::load(filename, data);
}

EnvironmentMap::~EnvironmentMap()
{
  delete data.cols;
}

Spectrum EnvironmentMap::getIntensity(HitRecord &hit, QVector3D &direction, const Intersectable &scene, Sample sample) const
{
  double pdf;
  direction = sample.getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
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
//    double at = atan(sqrt(direction.x() / (1 - direction.x())));
//    return coefficient * M_PI / at * Spectrum(data.cols[index], data.cols[index + 1],data.cols[index + 2]);
    return coefficient * Spectrum(data.cols[index], data.cols[index + 1],data.cols[index + 2]) / pdf;
  }
}
