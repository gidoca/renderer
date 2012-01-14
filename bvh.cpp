#include "bvh.h"

#include "intersectablelist.h"
#include "vechelper.h"

#include <list>

#include <QVector3D>
#include <QSharedPointer>

using namespace std;

BVHNode::BVHNode(Intersectable* left, Intersectable* right, AxisAlignedBox* bb) : bb(bb), left(left), right(right)
{

}

Intersectable* BVHNode::create(IntersectableList* list, int maxDepth)
{
  std::list< QSharedPointer<Intersectable> > intersectables = list->getComponents();
  if(intersectables.size() == 1 || maxDepth == 0) return list;
  
  AxisAlignedBox * bb = list->boundingBox();
  QVector3D diff = bb->getMax() - bb->getMin();
  int splitAxis = (diff.x() > diff.y() && diff.x() > diff.z() ? 0 : (diff.y() > diff.z() ? 1 : 2));
  QVector3D splitPlanes = bb->getMin() + diff / 2;
  float splitPlane = get(splitPlanes, splitAxis);
  
  std::list< QSharedPointer<Intersectable> > left, right;
  for(std::list< QSharedPointer<Intersectable> >::const_iterator i = intersectables.begin(); i != intersectables.end(); i++)
  {
    AxisAlignedBox * curbb = (*i)->boundingBox();
    float min, max;
    min = get(curbb->getMin(), splitAxis);
    max = get(curbb->getMax(), splitAxis);
    if(min < splitPlane) left.push_back(*i);
    if(splitPlane < max) right.push_back(*i);
    delete curbb;
  }
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


