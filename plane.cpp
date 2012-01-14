#include "plane.h"

#include "axisalignedbox.h"

#include <limits>

HitRecord Plane::intersect(Ray ray, float from, float to) const
{
  float d = -QVector3D::dotProduct(ray.getDirection(), planeVector.toVector3D());
  float u = QVector4D::dotProduct(QVector4D(ray.getOrigin(), 1), planeVector);
  float t = u / d;
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
  float inf = std::numeric_limits< float >::infinity();
  return new AxisAlignedBox(QVector3D(-inf, -inf, -inf), QVector3D(inf, inf, inf), DarkMatter::getInstance());
}
