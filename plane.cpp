#include "plane.h"

#include "axisalignedbox.h"

#include <limits>

HitRecord Plane::intersect(Ray ray, double from, double to) const
{
  double d = -QVector3D::dotProduct(ray.getDirection(), planeVector.toVector3D());
  double u = QVector4D::dotProduct(QVector4D(ray.getOrigin(), 1), planeVector);
  double t = u / d;
  if(from < t && t < to)
  {
    return HitRecord(t, ray, material, planeVector.toVector3D());
  }
  else
  {
    return HitRecord();
  }
}

AxisAlignedBox* Plane::boundingBox() const
{
  double inf = std::numeric_limits< double >::infinity();
  return new AxisAlignedBox(QVector3D(-inf, -inf, -inf), QVector3D(inf, inf, inf), DarkMatter::getInstance());
}
