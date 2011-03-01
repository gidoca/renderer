#include "sphere.h"

#include <cmath>

Sphere::Sphere(QVector3D center, double radius) : center(center), radius(radius)
{

}

HitRecord Sphere::intersect(Ray ray) const
{
  QVector3D diff = ray.getOrigin() - center;
  double radiusSquared = radius * radius;
  double a = ray.getDirection().lengthSquared();
  double b = 2 * QVector3D::dotProduct(ray.getDirection(), diff);
  double c = diff.lengthSquared() - radiusSquared;
  if(b * b - 4 * a * c < 0)
  {
    return HitRecord::noIntersection();
  }
  else
  {
    double rayParameter = -b - pow(b * b - 4 * a * c, 0.5) / (2 * a);
    return HitRecord(rayParameter, ray);
  }
}
