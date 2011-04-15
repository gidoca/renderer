#include "sphere.h"

#include <cmath>

#include "axisalignedbox.h"

HitRecord Sphere::intersect(Ray ray, double from, double to) const
{
  QVector3D diff = ray.getOrigin() - center;
  double radiusSquared = radius * radius;
  double a = ray.getDirection().lengthSquared();
  double b = 2 * QVector3D::dotProduct(ray.getDirection(), diff);
  double c = diff.lengthSquared() - radiusSquared;
  if(b * b - 4 * a * c < 0)
  {
    return HitRecord();
  }
  else
  {
    double rayParameter = (-b - pow(b * b - 4 * a * c, 0.5)) / (2 * a);
    if(from >= rayParameter || rayParameter >= to)
    {
      return HitRecord();
    }
    
    QVector3D location = ray.evaluate(rayParameter);
    return HitRecord(rayParameter, ray, material, location - center);
  }
}

AxisAlignedBox* Sphere::boundingBox() const
{
  QVector3D radiusVector(radius, radius, radius);
  return new AxisAlignedBox(center - radiusVector, center + radiusVector, DarkMatter::getInstance());
}
