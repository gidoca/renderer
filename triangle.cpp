#include "triangle.h"

#include <iostream>

#include <QMatrix4x4>

Triangle::Triangle(QVector3D p1, QVector3D p2, QVector3D p3) : p1(p1), p2(p2), p3(p3)
{
  
}

HitRecord Triangle::intersect(Ray ray) const
{
  Plane plane = Plane::fromPoints(p1, p3, p2);
  HitRecord hit = plane.intersect(ray);
  std::cout << "1: " << hit.getRayParameter();
  QMatrix4x4 intersectionMatrix;
  intersectionMatrix.setColumn(0, p1 - p2);
  intersectionMatrix.setColumn(1, p1 - p3);
  intersectionMatrix.setColumn(2, ray.getDirection());
  QVector4D result = intersectionMatrix.inverted().map(p1 - ray.getOrigin());
  std::cout << "; 2: " << result.z() << std::endl;
  if(result.z() > 0 && 0 < result.x() + result.y() && result.x() + result.y() < 1)
  {
    return HitRecord(result.z(), ray);
  }
  else
  {
    return HitRecord::noIntersection();
  }
}
