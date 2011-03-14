#include "triangle.h"

#include <iostream>

#include <QMatrix4x4>

#include <QVector3D>

Triangle::Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QSharedPointer<Material> material) : p1(p1), p2(p2), p3(p3), n1(QVector3D::crossProduct(p1 - p2, p1 - p3)), n2(n1), n3(n1), material(material)
{
  
}

Triangle::Triangle(QVector3D p1, QVector3D p2, QVector3D p3, QVector3D n1, QVector3D n2, QVector3D n3, QSharedPointer< Material > material) : p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3), material(material)
{

}

HitRecord Triangle::intersect(Ray ray) const
{
  if(QVector3D::dotProduct(ray.getDirection().toVector3D(), n1) > 0) return HitRecord();
  QMatrix4x4 intersectionMatrix;
  intersectionMatrix.setColumn(0, p1 - p2);
  intersectionMatrix.setColumn(1, p1 - p3);
  intersectionMatrix.setColumn(2, ray.getDirection());
  QVector4D result = intersectionMatrix.inverted().map(p1 - ray.getOrigin());
  
  double beta = result.x(), gamma = result.y(), alpha = 1 - beta - gamma;

  if(result.z() > 0 &&
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
