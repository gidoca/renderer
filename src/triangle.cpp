/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "triangle.h"

#include <iostream>

#include <QtGui/QMatrix4x4>

#include <QtGui/QVector3D>
#include "axisalignedbox.h"

HitRecord Triangle::intersect(Ray ray) const
{
    QVector3D ad = ray.getOrigin() - p1;
    QVector3D abxac = QVector3D::crossProduct(p2 - p1, p3 - p1);

    float w = -QVector3D::dotProduct(abxac, ray.getDirection());
    if(w == 0) return HitRecord();
    float t = QVector3D::dotProduct(abxac, ad) / w;
    if(t < ray.getFrom() || t > ray.getTo()) return HitRecord();

    QVector3D adxde = QVector3D::crossProduct(ad, ray.getDirection());
    float u = QVector3D::dotProduct(adxde, p3 - p1) / w;
    if(u < 0 || u > 1) return HitRecord();
    float v = -QVector3D::dotProduct(adxde, p2 - p1) / w;
    if(v < 0 || u + v > 1) return HitRecord();

    QVector3D normal = (1 - u - v) * n1 + u * n2 + v * n3;
    if(normal == QVector3D())
    {
        return HitRecord();
    }
    else
    {
        assert(ray.getFrom() <= t && t <= ray.getTo());
        return HitRecord(t, ray, material, normal, (1 - u - v) * t1 + u * t2 + v * t3);
    }
}

const AxisAlignedBox * Triangle::createBoundingBox()
{
  AxisAlignedBox * result = new AxisAlignedBox();
  result->includePoint(p1);
  result->includePoint(p2);
  result->includePoint(p3);
  return result;
}
