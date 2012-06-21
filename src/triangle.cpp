#include "triangle.h"

#include <iostream>

#include <QtGui/QMatrix4x4>

#include <QtGui/QVector3D>
#include "axisalignedbox.h"

HitRecord Triangle::intersect(Ray ray, float from, float to) const
{
//  if(QVector3D::dotProduct(ray.getDirection(), QVector3D::crossProduct(p1 - p2, p1 - p3)) > 0) return HitRecord();
  
  /*QVector3D e1 = p2 - p1;
  QVector3D e2 = p3 - p1;
  QVector3D s1 = QVector3D::crossProduct(ray.getDirection(), e2);
  float divisor = QVector3D::dotProduct(s1, e1);

  if(divisor == 0) return HitRecord();
  float invDivisor = 1. / divisor;
  
  QVector3D d = ray.getOrigin() - p1;
  float b1 = QVector3D::dotProduct(d, s1) * invDivisor;
  if(b1 < 0 || b1 > 1.) return HitRecord();
  
  QVector3D s2 = QVector3D::crossProduct(d, e1);
  float b2 = QVector3D::dotProduct(ray.getDirection(), s2) * invDivisor;
  if(b2 < 0 || b1 + b2 > 1.) return HitRecord();
  
  float t = QVector3D::dotProduct(e2, s2);
  if(t < from || t > to) return HitRecord();
  return HitRecord(t, ray, material, b1 * n1 + b2 * n2 + (1 - b1 - b2) * n3);*/
  
  QMatrix4x4 intersectionMatrix;
  intersectionMatrix.setColumn(0, p1 - p2);
  intersectionMatrix.setColumn(1, p1 - p3);
  intersectionMatrix.setColumn(2, ray.getDirection());
  QVector4D result = intersectionMatrix.inverted().map(p1 - ray.getOrigin());
  
  float beta = result.x(), gamma = result.y(), alpha = 1 - beta - gamma;

  if(from < result.z() && result.z() < to &&
    0 < alpha && alpha < 1 &&
    0 < beta && beta < 1 &&
    0 < gamma && gamma < 1)
  {
    return HitRecord(result.z(), ray, material, alpha * n1 + beta * n2 + gamma * n3, alpha * t1 + beta * t2 + gamma * t3);
  }
  else
  {
    return HitRecord();
  }
}

AxisAlignedBox * Triangle::boundingBox() const
{
  AxisAlignedBox * result = new AxisAlignedBox();
  result->includePoint(p1);
  result->includePoint(p2);
  result->includePoint(p3);
  return result;
}
