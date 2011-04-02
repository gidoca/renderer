#include "axisalignedbox.h"

#include "material.h"
#include "darkmatter.h"

#include <cmath>

AxisAlignedBox::AxisAlignedBox() :
  min(QVector3D(std::numeric_limits< double >::infinity(), std::numeric_limits< double >::infinity(), std::numeric_limits< double >::infinity())),
  max(QVector3D(-std::numeric_limits< double >::infinity(), -std::numeric_limits< double >::infinity(), -std::numeric_limits< double >::infinity())),
  material(QSharedPointer<Material>(new DarkMatter()))
{

}


AxisAlignedBox::AxisAlignedBox(QVector3D min, QVector3D max, QSharedPointer<Material> material): min(min), max(max), material(material)
{
  
}

AxisAlignedBox::AxisAlignedBox(const AxisAlignedBox& other): min(other.min), max(other.max), material(other.material)
{

}


HitRecord AxisAlignedBox::intersect(Ray ray, double from, double to) const
{
  double a;
  double tmin, tmax;
  double temp_tmin, temp_tmax;
  QVector3D rayDirection = ray.getDirection().toVector3D();
  QVector3D rayOrigin = ray.getOrigin().toVector3DAffine();
  QVector3D surfaceNormal;
  a = 1 / rayDirection.x();
  if(a >= 0)
  {
    tmin = (min.x() - rayOrigin.x());
    tmax = (max.x() - rayOrigin.x());
  }
  else
  {
    tmin = (max.x() - rayOrigin.x());
    tmax = (min.x() - rayOrigin.x());
  }
  tmin *= a;
  tmax *= a;
  a = 1 / rayDirection.y();
  if(a >= 0)
  {
    temp_tmin = (min.y() - rayOrigin.y());
    temp_tmax = (max.y() - rayOrigin.y());
  }
  else
  {
    temp_tmin = (max.y() - rayOrigin.y());
    temp_tmax = (min.y() - rayOrigin.y());
  }
  temp_tmin *= a;
  temp_tmax *= a;
  if(temp_tmin > tmin) tmin = temp_tmin;
  if(temp_tmax < tmax) tmax = temp_tmax;
  a = 1 / rayDirection.z();
  if(a >= 0)
  {
    temp_tmin = (min.z() - rayOrigin.z());
    temp_tmax = (max.z() - rayOrigin.z());
  }
  else
  {
    temp_tmin = (max.z() - rayOrigin.z());
    temp_tmax = (min.z() - rayOrigin.z());
  }
  temp_tmin *= a;
  temp_tmax *= a;
  if(temp_tmin > tmin) tmin = temp_tmin;
  if(temp_tmax < tmax) tmax = temp_tmax;
  if(tmin < tmax)
  {
    return HitRecord(tmin, ray, material, QVector3D());
  }
  else
  {
    return HitRecord();
  }
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
