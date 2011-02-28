#include "sphere.h"

Sphere::Sphere(QVector3D center, double radius) : center(center), radius(radius)
{

}

//TODO fixme
bool Sphere::intersect(Ray ray) const
{
  QVector3D diff = ray.getOrigin() - center;
  double radiusSquared = radius * radius;
  double a = ray.getDirection().lengthSquared();
  double b = 2 * QVector3D::dotProduct(ray.getDirection(), diff);
  double c = diff.lengthSquared() - radiusSquared;
  return b * b - 2 * a * c >= 0;
}
