#include "triangle.h"

#include <iostream>

#include <QtGui/QMatrix4x4>

#include <QtGui/QVector3D>
#include "axisalignedbox.h"

HitRecord Triangle::intersect(Ray ray, double from, double to) const
{
  if(QVector3D::dotProduct(ray.getDirection(), QVector3D::crossProduct(p1 - p2, p1 - p3)) > 0) return HitRecord();
  
  /*QVector3D edge1 = p1 - p2;
  QVector3D edge2 = p1 - p3;
  QVector3D pvec = QVector3D::crossProduct(ray.getDirection().toVector3D(), edge2);
  double det = QVector3D::dotProduct(pvec, edge1);
  if(det < EPSILON) return HitRecord();
  
  QVector3D tvec = ray.getOrigin().toVector3DAffine() - p1;
  double u = QVector3D::dotProduct(pvec, tvec);
  if(u < 0 || u > det) return HitRecord();
  
  QVector3D qvec = QVector3D::crossProduct(tvec, edge1);
  double v = QVector3D::dotProduct(ray.getDirection().toVector3D(), qvec);
  if(v < 0 || u + v > det) return HitRecord();
  
  double t = QVector3D::dotProduct(edge2, qvec);
  double inv_det = 1 / det;
  t *= inv_det;
  u *= inv_det;
  v *= inv_det;
  return HitRecord(t, ray, material, u * n1 + v * n2 + (1 - u - v) * n3);*/
  
  QMatrix4x4 intersectionMatrix;
  intersectionMatrix.setColumn(0, p1 - p2);
  intersectionMatrix.setColumn(1, p1 - p3);
  intersectionMatrix.setColumn(2, ray.getDirection());
  QVector4D result = intersectionMatrix.inverted().map(p1 - ray.getOrigin());
  
  double beta = result.x(), gamma = result.y(), alpha = 1 - beta - gamma;

  if(from < result.z() && result.z() < to &&
    0 < alpha && alpha < 1 &&
    0 < beta && beta < 1 &&
    0 < gamma && gamma < 1)
  {
    return HitRecord(result.z(), ray, material, alpha * n1 + beta * n2 + gamma * n3);
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
