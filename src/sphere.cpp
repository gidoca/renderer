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
#include "sphere.h"

#include <cmath>
#include <limits>

#include "axisalignedbox.h"

IntersectionParameter Sphere::getCSGIntersection(Ray ray) const
{
  IntersectionParameter result;
  QVector3D diff = ray.getOrigin() - center;
  float radiusSquared = radius * radius;
  float a = ray.getDirection().lengthSquared();
  float b = 2 * QVector3D::dotProduct(ray.getDirection(), diff);
  float c = diff.lengthSquared() - radiusSquared;
  if(b * b - 4 * a * c >= 0)
  {
    result.intersections.push_back((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
    result.intersections.push_back((-b + sqrt(b * b - 4 * a * c)) / (2 * a));

    QVector3D location = ray.evaluate(result.intersections.front());
    result.normal = location - center;
  }
  result.material = material;
  return result;
}

AxisAlignedBox* Sphere::boundingBox() const
{
  QVector3D radiusVector(radius, radius, radius);
  return new AxisAlignedBox(center - radiusVector, center + radiusVector, DarkMatter::getInstance());
}
