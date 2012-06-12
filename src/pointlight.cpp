#include "pointlight.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "sampler.h"

#include <cmath>

cv::Vec3f PointLight::getIntensity (const QVector3D& at, QVector3D& direction, const Intersectable& scene, const Sample&) const
{
  direction = getDirection(at);
  if(isOccluded(at, scene))
  {
    return cv::Vec3f();
  }
  else
  {
    return intensity * (1 / direction.lengthSquared());
  }
}

Ray PointLight::getRandomRay(const Sample& sample, const Sample&, float& pdf) const
{
  //TODO verify
  pdf = 1;
  return Ray(location, sample.getUniformSphereDirection());
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
