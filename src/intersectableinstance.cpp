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
#include "intersectableinstance.h"
#include "axisalignedbox.h"

HitRecord IntersectableInstance::intersect(Ray ray) const
{
  Ray transformedRay = ray.transform(inverseTransform);
  HitRecord hitRecord = intersectable->intersect(transformedRay);
  hitRecord.transform(transform);
  return hitRecord;
}

const AxisAlignedBox * IntersectableInstance::createBoundingBox()
{
  const AxisAlignedBox * untransformed = intersectable->boundingBox();
  AxisAlignedBox * result = new AxisAlignedBox();
  QVector3D min = untransformed->getMin(), max = untransformed->getMax();
  result->includePoint(transform.map(QVector3D(min.x(), min.y(), min.z())));
  result->includePoint(transform.map(QVector3D(min.x(), min.y(), max.z())));
  result->includePoint(transform.map(QVector3D(min.x(), max.y(), min.z())));
  result->includePoint(transform.map(QVector3D(min.x(), max.y(), max.z())));
  result->includePoint(transform.map(QVector3D(max.x(), min.y(), min.z())));
  result->includePoint(transform.map(QVector3D(max.x(), min.y(), max.z())));
  result->includePoint(transform.map(QVector3D(max.x(), max.y(), min.z())));
  result->includePoint(transform.map(QVector3D(max.x(), max.y(), max.z())));
  return result;
}
