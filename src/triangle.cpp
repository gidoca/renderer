#include "triangle.h"

#include <iostream>

#include <QtGui/QMatrix4x4>

#include <QtGui/QVector3D>
#include "axisalignedbox.h"

HitRecord Triangle::intersect(Ray ray, float from, float to) const
{
    QVector3D ad = ray.getOrigin() - p1;
    QVector3D adxde = QVector3D::crossProduct(ad, ray.getDirection());
    QVector3D abxac = QVector3D::crossProduct(p2 - p1, p3 - p1);

    float w = -QVector3D::dotProduct(abxac, ray.getDirection());
    if(w == 0) return HitRecord();
    float t = QVector3D::dotProduct(abxac, ad) / w;
    if(t < from || t > to) return HitRecord();
    float u = QVector3D::dotProduct(adxde, p3 - p1) / w;
    if(u < 0 || u > 1) return HitRecord();
    float v = -QVector3D::dotProduct(adxde, p2 - p1) / w;
    if(v < 0 || u + v > 1) return HitRecord();

    return HitRecord(t, ray, material, (1 - u - v) * n1 + u * n2 + v * n3, (1 - u - v) * t1 + u * t2 + v * t3);
}

AxisAlignedBox * Triangle::boundingBox() const
{
  AxisAlignedBox * result = new AxisAlignedBox();
  result->includePoint(p1);
  result->includePoint(p2);
  result->includePoint(p3);
  return result;
}
