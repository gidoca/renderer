#include "csgintersection.h"

#include "axisalignedbox.h"

#include <algorithm>

IntersectionParameter CSGIntersection::getCSGIntersection(Ray ray) const
{
  bool leftInside = false, rightInside = false;
  IntersectionParameter leftIntersection = left->getCSGIntersection(ray);
  IntersectionParameter rightIntersection = right->getCSGIntersection(ray);

  IntersectionParameter result;

  std::list<double>::iterator leftIterator = leftIntersection.intersections.begin();
  std::list<double>::iterator rightIterator = rightIntersection.intersections.begin();

  if(*leftIterator > *rightIterator)
  {
    result.material = leftIntersection.material;
    result.normal = leftIntersection.normal;
  }
  else
  {
    result.material = rightIntersection.material;
    result.normal = rightIntersection.normal;
  }

  while(leftIterator != leftIntersection.intersections.end() && rightIterator != rightIntersection.intersections.end())
  {
    if(*leftIterator <= *rightIterator)
    {
      leftInside = !leftInside;
      if(leftInside && rightInside)
      {
        result.intersections.push_back(*leftIterator);
      }
      leftIterator++;
    }
    else
    {
      rightInside = !rightInside;
      if(leftInside && rightInside)
      {
        result.intersections.push_back(*leftIterator);
      }
      rightIterator++;
    }
    if(leftInside && rightInside)
    {
      result.intersections.push_back(std::min(*leftIterator, *rightIterator));
    }
  }

  return result;
}

AxisAlignedBox * CSGIntersection::boundingBox() const
{
  AxisAlignedBox * leftBB = left->boundingBox();
  AxisAlignedBox * rightBB = right->boundingBox();
  QVector3D min(std::max(leftBB->getMin().x(), rightBB->getMin().x()),
                std::max(leftBB->getMin().y(), rightBB->getMin().y()),
                std::max(leftBB->getMin().z(), rightBB->getMin().z()));
  QVector3D max(std::min(leftBB->getMax().x(), rightBB->getMax().x()),
                std::min(leftBB->getMax().y(), rightBB->getMax().y()),
                std::min(leftBB->getMax().z(), rightBB->getMax().z()));
  AxisAlignedBox * result = new AxisAlignedBox(min, max);
  return result;
}
