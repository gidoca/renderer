#include "bsp.h"
#include "intersectablelist.h"
#include "plane.h"

#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

int BSPNode::cnt = 0;


BSPNode::BSPNode(AxisAlignedBox* boundingBox): bBox(boundingBox)
{
  BSPNode::cnt++;
}


BSPNode::~BSPNode()
{
  //delete bBox;
}


BSPNode * BSPNode::buildTree(IntersectableList * intersectables)
{
//  return buildTree(intersectables, 0, 8 + (int) (1.3 * log(intersectables->getComponents().size())), intersectables->boundingBox());
  return buildTree(intersectables, 0, 2, intersectables->boundingBox());
}

BSPNode* BSPNode::buildTree(IntersectableList * intersectables, int depth, int maxDepth, AxisAlignedBox * boundingBox)
{
  std::list< QSharedPointer<Intersectable> > components = intersectables->getComponents();
  if(depth >= maxDepth || components.size() <= 1)
  {
    return new BSPLeafNode(intersectables, boundingBox);
  }
  else
  {
    std::list< QSharedPointer<Intersectable> > leftList, rightList;
    QVector3D diff = boundingBox->getMax() - boundingBox->getMin();
    QVector3D splitMin = boundingBox->getMin(), splitMax = boundingBox->getMax();
    short axis;
    double planePosition;
    if(diff.x() >= diff.z())
    {
      if(diff.x() >= diff.y())
      {
        axis = 0;
        planePosition = boundingBox->getMin().x() + diff.x() / 2;
        splitMin.setX(planePosition);
        splitMax.setX(planePosition);
        for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
        {
          AxisAlignedBox * componentBoundingBox = (*i)->boundingBox();
          if(planePosition <= componentBoundingBox->getMax().x())
          {
            rightList.push_back(*i);
          }
          if(planePosition >= componentBoundingBox->getMin().x())
          {
            leftList.push_back(*i);
          }
          delete componentBoundingBox;
        }
      }
      else
      {
        axis = 1;
        planePosition = boundingBox->getMin().y() + diff.y() / 2;
        splitMin.setY(planePosition);
        splitMax.setY(planePosition);
        for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
        {
          AxisAlignedBox * componentBoundingBox = (*i)->boundingBox();
          if(planePosition <= componentBoundingBox->getMax().y())
          {
            rightList.push_back(*i);
          }
          if(planePosition >= componentBoundingBox->getMin().y())
          {
            leftList.push_back(*i);
          }
          delete componentBoundingBox;
        }
      }
    }
    else
    {
      axis = 2;
      planePosition = boundingBox->getMin().z() + diff.z() / 2;
      splitMin.setZ(planePosition);
      splitMax.setZ(planePosition);
      for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
      {
        AxisAlignedBox * componentBoundingBox = (*i)->boundingBox();
        if(planePosition <= componentBoundingBox->getMax().z())
        {
          rightList.push_back(*i);
        }
        if(planePosition >= componentBoundingBox->getMin().z())
        {
          leftList.push_back(*i);
        }
        delete componentBoundingBox;
      }
    }
    QSharedPointer<Material> mat = DarkMatter::getInstance();
    AxisAlignedBox * leftBoundingBox = new AxisAlignedBox(boundingBox->getMin(), splitMax, mat);
    AxisAlignedBox * rightBoundingBox = new AxisAlignedBox(splitMin, boundingBox->getMax(), mat);
    BSPNode * leftNode = buildTree(new IntersectableList(leftList), depth + 1, maxDepth, leftBoundingBox);
    BSPNode * rightNode = buildTree(new IntersectableList(rightList), depth + 1, maxDepth, rightBoundingBox);
    BSPInternalNode * result = new BSPInternalNode(planePosition, axis, leftNode, rightNode, boundingBox);
    delete boundingBox;
    delete intersectables;
    return result;
  }
}

AxisAlignedBox* BSPNode::boundingBox() const
{
  return new AxisAlignedBox(*bBox);
}

BSPLeafNode::BSPLeafNode(IntersectableList* objects, AxisAlignedBox * boundingBox): BSPNode(boundingBox), objects(objects)
{
  
}

BSPLeafNode::~BSPLeafNode()
{

}

HitRecord BSPLeafNode::intersect(Ray ray, double from, double to) const
{
  return objects->intersect(ray, from, to);
}

BSPInternalNode::BSPInternalNode(double planePosition, short int axis, BSPNode* lowerNode, BSPNode* upperNode, AxisAlignedBox* boundingBox): BSPNode(boundingBox), planePosition(planePosition), axis(axis), lowerNode(lowerNode), upperNode(upperNode)
{
  
}

BSPInternalNode::~BSPInternalNode()
{
  delete lowerNode;
  delete upperNode;
}

HitRecord BSPInternalNode::intersect(Ray ray, double from, double to) const
{
  IntersectionParameter intersection = bBox->getIntersectionParameter(ray);
//  if(intersection.tmin >= intersection.tmax) return HitRecord();
//  if(intersection.tmin < intersection.tmax) std::cout << "I";
  BSPNode *first, *second;
  double rayOrigin, rayDirection;
  QVector4D splitPlaneVector;
  switch(axis)
  {
  case 0:
    rayOrigin = ray.getOrigin().x();
    rayDirection = ray.getDirection().x();
    splitPlaneVector = QVector4D(-1, 0, 0, planePosition);
    break;
  case 1:
    rayOrigin = ray.getOrigin().y();
    rayDirection = ray.getDirection().y();
    splitPlaneVector = QVector4D(0, -1, 0, planePosition);
    break;
  case 2:
    rayOrigin = ray.getOrigin().z();
    rayDirection = ray.getDirection().z();
    splitPlaneVector = QVector4D(0, 0, -1, planePosition);
    break;
  }
  
  Plane splitPlane(splitPlaneVector, DarkMatter::getInstance());
  HitRecord splitPlaneHit = splitPlane.intersect(ray, -std::numeric_limits<double>::infinity());
  double tsplit = splitPlaneHit.getRayParameter();
  bool rayTowardsFirst;
  
  if(rayOrigin < planePosition)
  {
    first = lowerNode;
    second = upperNode;
    rayTowardsFirst = rayDirection < 0;
  }
  else
  {
    first = upperNode;
    second = lowerNode;
    rayTowardsFirst = rayDirection > 0;
  }

  HitRecord firstHit = first->intersect(ray, from, to);
  HitRecord secondHit = second->intersect(ray, from, to);
  if(firstHit.getRayParameter() < secondHit.getRayParameter())
  {
    return firstHit;
  }
  else
  {
    return secondHit;
  }
  
  /*if(tsplit > intersection.tmax || tsplit < 0 || (tsplit == 0 && rayTowardsFirst))
  {
    return first->intersect(ray, max(from, intersection.tmin), min(to, intersection.tmax));
  }
  else if(tsplit < intersection.tmin || (tsplit == 0 && !rayTowardsFirst))
  {
    return second->intersect(ray, max(from, intersection.tmin), min(to, intersection.tmax));
  }
  else
  {
//    HitRecord firstHit = first->intersect(ray, (from > intersection.tmin ? from : intersection.tmin), tsplit);
    HitRecord firstHit = first->intersect(ray, from, tsplit);
    if(firstHit.intersects())
    {
      return firstHit;
    }
    else
    {
//      return second->intersect(ray, tsplit, intersection.tmax);
      return second->intersect(ray, tsplit, to);
    }
  }*/
  /*AxisAlignedBox * bb = first->boundingBox();
  if(bb->intersect(ray, from, to).intersects())
  {
    HitRecord firstHit = first->intersect(ray, max(from, intersection.tmin), min(min(to, intersection.tmax), tsplit));
    if(firstHit.intersects()) return firstHit;
  }
//  delete bb;
  bb = second->boundingBox();
  bool intersects = bb->intersect(ray, from, to).intersects();
//  delete bb;
  if(intersects)
  {
    return second->intersect(ray, max(max(from, tsplit), intersection.tmin), min(to, intersection.tmax));
  }
  else
  {
    return HitRecord();
  }*/
}
