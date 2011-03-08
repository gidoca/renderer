#include "sphere.h"

#include <cmath>

Sphere::Sphere(QVector3D center, double radius, QSharedPointer<Material> material) : center(center), radius(radius), material(material)
{

}

HitRecord Sphere::intersect(Ray ray) const
{
  QVector3D diff = ray.getOrigin().toVector3DAffine() - center;
  double radiusSquared = radius * radius;
  double a = ray.getDirection().lengthSquared();
  double b = 2 * QVector3D::dotProduct(ray.getDirection().toVector3D(), diff);
  double c = diff.lengthSquared() - radiusSquared;
  if(b * b - 4 * a * c < 0)
  {
    return HitRecord();
  }
  else
  {
    double rayParameter = -b - pow(b * b - 4 * a * c, 0.5) / (2 * a);
    QVector3D location = ray.evaluate(rayParameter);
    return HitRecord(rayParameter, ray, material, location - center);
  }
}

