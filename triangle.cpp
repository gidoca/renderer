#include "triangle.h"

#include <iostream>

#include <QMatrix4x4>

Triangle::Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QSharedPointer<Material> material) : p1(p1), p2(p2), p3(p3), material(material)
{
  
}

HitRecord Triangle::intersect(Ray ray) const
{
  QMatrix4x4 intersectionMatrix;
  intersectionMatrix.setColumn(0, p1 - p2);
  intersectionMatrix.setColumn(1, p1 - p3);
  intersectionMatrix.setColumn(2, ray.getDirection());
  QVector4D result = intersectionMatrix.inverted().map(p1 - ray.getOrigin());

  if(result.z() > 0 &&
    0 < result.x() + result.y() && result.x() + result.y() < 1 &&
    0 < result.x() && result.x() < 1 &&
    0 < result.y() && result.y() < 1)
  {
    return HitRecord(result.z(), ray, material);
  }
  else
  {
    return HitRecord();
  }
}
