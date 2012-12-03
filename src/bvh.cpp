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
#include <algorithm>
#include <iostream>

#include <QVector3D>

using namespace std;

class IntersectableComparator
{
public:
    IntersectableComparator(int splitAxis) : splitAxis(splitAxis) {}

    bool operator()(Intersectable* a, Intersectable* b)
    {
        AxisAlignedBox *bb1 = a->boundingBox(), *bb2 = b->boundingBox();

        float leftCenter = get(bb1->getMax() + bb1->getMin(), splitAxis) / 2;
        float rightCenter = get(bb2->getMax() + bb2->getMin(), splitAxis) / 2;
        delete bb1;
        delete bb2;
        return leftCenter < rightCenter;
    }

private:
    int splitAxis;
};

BVHNode::BVHNode(Intersectable* left, Intersectable* right, AxisAlignedBox* bb) : bb(bb), left(left), right(right)
{

}

Intersectable* BVHNode::create(IntersectableList* list)
{

  std::vector< Intersectable* > intersectables = list->getComponents();
  if(intersectables.size() == 1) return intersectables.front();
  
  AxisAlignedBox * bb = list->boundingBox();
  QVector3D diff = bb->getMax() - bb->getMin();
  int splitAxis = (diff.x() > diff.y() && diff.x() > diff.z() ? 0 : (diff.y() > diff.z() ? 1 : 2));
  IntersectableComparator comparator(splitAxis);
  sort(intersectables.begin(), intersectables.end(), comparator);
  

  auto it0 = intersectables.begin(), it1 = intersectables.begin() + intersectables.size() / 2, it2 = intersectables.end();
  std::vector< Intersectable* > left(it0, it1), right(it1, it2);

  if(left.size() == 0 || right.size() == 0) return list;

  //TODO delete all those intermediate lists
  BVHNode* node = new BVHNode(create(new IntersectableList(left)), create(new IntersectableList(right)), bb);
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


