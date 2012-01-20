#include "sphere.h"

#include <cmath>
#include <limits>

#include "axisalignedbox.h"

IntersectionParameter Sphere::getCSGIntersection(Ray ray) const
{
  IntersectionParameter result;
  QVector3D diff = ray.getOrigin() - center;
  float radiusSquared = radius * radius;
  float a = ray.getDirection().lengthSquared();
  float b = 2 * QVector3D::dotProduct(ray.getDirection(), diff);
  float c = diff.lengthSquared() - radiusSquared;
  if(b * b - 4 * a * c >= 0)
  {
    result.intersections.push_back((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
    result.intersections.push_back((-b + sqrt(b * b - 4 * a * c)) / (2 * a));

    QVector3D location = ray.evaluate(result.intersections.front());
    result.normal = location - center;
  }
  result.material = material;
  return result;
}

AxisAlignedBox* Sphere::boundingBox() const
{
  QVector3D radiusVector(radius, radius, radius);
  return new AxisAlignedBox(center - radiusVector, center + radiusVector, DarkMatter::getInstance());
}
