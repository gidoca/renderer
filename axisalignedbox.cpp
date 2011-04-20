#include "axisalignedbox.h"

#include "material.h"
#include "darkmatter.h"

#include <cmath>

IntersectionParameter AxisAlignedBox::getCSGIntersection(Ray ray) const
{
  IntersectionParameter result;
  double a;
  double tmin, tmax, temp_tmin, temp_tmax;
  QVector3D rayDirection = ray.getDirection();
  QVector3D rayOrigin = ray.getOrigin();
  QVector3D temp_normal;
  a = 1 / rayDirection.x();
  if(a >= 0)
  {
    tmin = (min.x() - rayOrigin.x());
    tmax = (max.x() - rayOrigin.x());
    result.normal = QVector3D(-1, 0, 0);
  }
  else
  {
    tmin = (max.x() - rayOrigin.x());
    tmax = (min.x() - rayOrigin.x());
    result.normal = QVector3D(1, 0, 0);
  }
  tmin *= a;
  tmax *= a;
  a = 1 / rayDirection.y();
  if(a >= 0)
  {
    temp_tmin = (min.y() - rayOrigin.y());
    temp_tmax = (max.y() - rayOrigin.y());
    temp_normal = QVector3D(0, -1, 0);
  }
  else
  {
    temp_tmin = (max.y() - rayOrigin.y());
    temp_tmax = (min.y() - rayOrigin.y());
    temp_normal = QVector3D(0, 1, 0);
  }
  temp_tmin *= a;
  temp_tmax *= a;
  if(temp_tmin > tmin)
  {
    tmin = temp_tmin;
    result.normal = temp_normal;
  }
  if(temp_tmax < tmax) tmax = temp_tmax;
  a = 1 / rayDirection.z();
  if(a >= 0)
  {
    temp_tmin = (min.z() - rayOrigin.z());
    temp_tmax = (max.z() - rayOrigin.z());
    temp_normal = QVector3D(0, 0, -1);
  }
  else
  {
    temp_tmin = (max.z() - rayOrigin.z());
    temp_tmax = (min.z() - rayOrigin.z());
    temp_normal = QVector3D(0, 0, 1);
  }
  temp_tmin *= a;
  temp_tmax *= a;
  if(temp_tmin > tmin)
  {
    tmin = temp_tmin;
    result.normal = temp_normal;
  }
  if(temp_tmax < tmax) tmax = temp_tmax;
  if(tmin <= tmax)
  {
    result.intersections.push_back(tmin);
    result.intersections.push_back(tmax);
  }
  result.material = material;
  return result;
}

AxisAlignedBox* AxisAlignedBox::boundingBox() const
{
  return new AxisAlignedBox(*this);
}

QVector3D AxisAlignedBox::getMin() const
{
  return min;
}

QVector3D AxisAlignedBox::getMax() const
{
  return max;
}

void AxisAlignedBox::includePoint(QVector3D point)
{
  if(point.x() < min.x()) min.setX(point.x());
  if(point.y() < min.y()) min.setY(point.y());
  if(point.z() < min.z()) min.setZ(point.z());
  if(point.x() > max.x()) max.setX(point.x());
  if(point.y() > max.y()) max.setY(point.y());
  if(point.z() > max.z()) max.setZ(point.z());
}
