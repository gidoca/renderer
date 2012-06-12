#include "conelight.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "sampler.h"

#include <cmath>

using namespace cv;

Vec3f ConeLight::getIntensity(const QVector3D & at, QVector3D &direction, const Intersectable &scene, const Sample &) const
{
  direction = at - location;
  HitRecord shadowHit = scene.intersect(Ray(at, -direction.normalized()), EPSILON, direction.length());
  cv::Vec3f part = cv::Vec3f(1, 1, 1);
  float rayAngle = QVector3D::dotProduct(direction.normalized(), this->direction.normalized());
  if(0 <= acos(rayAngle) && acos(rayAngle) <= openingAngle && !shadowHit.intersects())
  {
    return intensity.mul(part) * (1 / direction.lengthSquared());
  }
  else
  {
    return Vec3f();
  }
}

Ray ConeLight::getRandomRay(const Sample &sample, const Sample&, float &pdf) const
{
  return Ray(location, sample.getCosineWeightedDirection(direction, pdf, openingAngle));
}
