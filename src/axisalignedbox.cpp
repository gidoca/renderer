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

std::list<IntersectionParameter> AxisAlignedBox::getCSGIntersection(Ray ray) const
{
  std::list<IntersectionParameter> result;
  IntersectionParameter param;
  param.material = material;
  float a;
  float tmin = -std::numeric_limits<float>::infinity(), tmax = std::numeric_limits<float>::infinity(), temp_tmin, temp_tmax;
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
      param.normal = temp_normal;
    }
    if(temp_tmax < tmax) tmax = temp_tmax;
  }

  if(tmin <= tmax)
  {
    param.t = tmin;
    result.push_back(param);
    param.t = tmax;
    result.push_back(param);
  }
  return result;
}

const AxisAlignedBox* AxisAlignedBox::createBoundingBox()
{
  return this;
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
    for(int i = 0; i < 3; i++)
    {
        float p = get(point, i);
        if(p < get(min, i)) set(min, i, p);
        if(p > get(max, i)) set(max, i, p);
    }
}

void AxisAlignedBox::includeOther(const AxisAlignedBox *box)
{
    includePoint(box->min);
    includePoint(box->max);
}
