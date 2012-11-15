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
#include "bvh.h"

#include "intersectablelist.h"
#include "vechelper.h"

#include <list>

#include <QVector3D>

using namespace std;

BVHNode::BVHNode(Intersectable* left, Intersectable* right, AxisAlignedBox* bb) : bb(bb), left(left), right(right)
{

}

Intersectable* BVHNode::create(IntersectableList* list, int maxDepth)
{
  std::list< Intersectable* > intersectables = list->getComponents();
  if(intersectables.size() == 1 || maxDepth == 0) return list;
  
  AxisAlignedBox * bb = list->boundingBox();
  QVector3D diff = bb->getMax() - bb->getMin();
  int splitAxis = (diff.x() > diff.y() && diff.x() > diff.z() ? 0 : (diff.y() > diff.z() ? 1 : 2));
  QVector3D splitPlanes = bb->getMin() + diff / 2;
  float splitPlane = get(splitPlanes, splitAxis);
  
  std::list< Intersectable* > left, right;
  for(std::list< Intersectable* >::const_iterator i = intersectables.begin(); i != intersectables.end(); i++)
  {
    AxisAlignedBox * curbb = (*i)->boundingBox();
    float min, max;
    min = get(curbb->getMin(), splitAxis);
    max = get(curbb->getMax(), splitAxis);
    if(min < splitPlane) left.push_back(*i);
    if(splitPlane < max) right.push_back(*i);
    delete curbb;
  }
  //TODO delete all those intermediate list
  BVHNode* node = new BVHNode(create(new IntersectableList(left), maxDepth - 1), create(new IntersectableList(right), maxDepth - 1), bb);
  return node;
}


AxisAlignedBox* BVHNode::boundingBox() const
{
  return new AxisAlignedBox(*bb);
}

HitRecord BVHNode::intersect(Ray ray, float from, float to) const
{
  if(!bb->intersect(ray, from, to).intersects()) return HitRecord();
  HitRecord lhit = left->intersect(ray, from, to);
  HitRecord rhit = right->intersect(ray, from, to);
  if(!rhit.intersects() || lhit.getRayParameter() < rhit.getRayParameter())
  {
    return lhit;
  }
  else
  {
    return rhit;
  }
}


