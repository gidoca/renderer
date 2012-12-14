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
#include "axisalignedbox.h"

#include "material.h"
#include "darkmatter.h"
#include "vechelper.h"

#include <cmath>
#include <limits>

IntersectionParameter AxisAlignedBox::getCSGIntersection(Ray ray) const
{
  IntersectionParameter result;
  float a;
  float tmin = 0, tmax = std::numeric_limits<float>::infinity(), temp_tmin, temp_tmax;
  QVector3D rayDirection = ray.getDirection();
  QVector3D rayOrigin = ray.getOrigin();
  QVector3D temp_normal;

  for(int i = 0; i < 3; i++)
  {
    a = 1 / get(rayDirection, i);
    if(a >= 0)
    {
      temp_tmin = get(min - rayOrigin, i);
      temp_tmax = get(max - rayOrigin, i);
      set(temp_normal, i, -1);
    }
    else
    {
      temp_tmin = get(max - rayOrigin, i);
      temp_tmax = get(min - rayOrigin, i);
      set(temp_normal, i, 1);
    }
    temp_tmin *= a;
    temp_tmax *= a;
    if(temp_tmin > tmin)
    {
      tmin = temp_tmin;
      result.normal = temp_normal;
    }
    if(temp_tmax < tmax) tmax = temp_tmax;
  }

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
