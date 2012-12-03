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
#include "intersectablelist.h"
#include "axisalignedbox.h"

#include <limits>

IntersectableList::~IntersectableList()
{
}

HitRecord IntersectableList::intersect(Ray ray, float from, float to) const
{
//  if(!bBox->intersect(ray, from, to).intersects()) return HitRecord();

  HitRecord hit;

  for(auto i = components.begin(); i != components.end(); i++)
  {
    const Intersectable & component = **i;
    HitRecord currentHit = component.intersect(ray, from, (to < hit.getRayParameter() ? to : hit.getRayParameter()));
    if(currentHit.intersects())
    {
      hit = currentHit;
    }
  }
  return hit;
}

AxisAlignedBox * IntersectableList::boundingBox() const
{
  QVector3D min(std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity(), std::numeric_limits< float >::infinity());
  QVector3D max = -min;
  for(auto i = components.begin(); i != components.end(); i++)
  {
    AxisAlignedBox * boundingBox = (*i)->boundingBox();
    QVector3D currentMin = boundingBox->getMin();
    QVector3D currentMax = boundingBox->getMax();
    delete boundingBox;
    if(currentMin.x() < min.x()) min.setX(currentMin.x());
    if(currentMin.y() < min.y()) min.setY(currentMin.y());   
    if(currentMin.z() < min.z()) min.setZ(currentMin.z());   
    if(currentMax.x() > max.x()) max.setX(currentMax.x());
    if(currentMax.y() > max.y()) max.setY(currentMax.y());   
    if(currentMax.z() > max.z()) max.setZ(currentMax.z());   
  }
  return new AxisAlignedBox(min, max, DarkMatter::getInstance());
}


std::vector< Intersectable* > IntersectableList::getComponents() const
{
  return components;
}
