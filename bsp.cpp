#include "bsp.h"
#include "intersectablelist.h"
#include "plane.h"

#include <cmath>

BSPNode::BSPNode(AxisAlignedBox* boundingBox): bBox(boundingBox)
{

}


BSPNode::~BSPNode()
{
  delete bBox;
}


BSPNode * BSPNode::buildTree(IntersectableList * intersectables)
{
  return buildTree(intersectables, 0, 8 + (int) (1.3 * log(intersectables->getComponents().size())));
}

BSPNode* BSPNode::buildTree(IntersectableList * intersectables, int depth, int maxDepth)
{
  std::list< QSharedPointer<Intersectable> > components = intersectables->getComponents();
  if(depth >= maxDepth || components.size() <= 1)
  {
    return new BSPLeafNode(intersectables);
  }
  else
  {
    std::list< QSharedPointer<Intersectable> > leftList, rightList;
    AxisAlignedBox * boundingBox = intersectables->boundingBox();
    QVector3D diff = boundingBox->getMax() - boundingBox->getMin();
    short axis;
    double planePosition;
    if(diff.x() >= diff.z())
    {
      if(diff.x() >= diff.y())
      {
	axis = 0;
	planePosition = boundingBox->getMin().x() + diff.x() / 2;
	for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
	{
	  AxisAlignedBox * componentBoundingBox = (*i)->boundingBox();
	  if(planePosition <= componentBoundingBox->getMax().x())
	  {
	    leftList.push_back(*i);
	  }
	  if(planePosition >= componentBoundingBox->getMin().x())
	  {
	    rightList.push_back(*i);
	  }
	  delete componentBoundingBox;
	}
      }
      else
      {
	axis = 1;
 	planePosition = boundingBox->getMin().y() + diff.y() / 2;
 	for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
	{
	  AxisAlignedBox * componentBoundingBox = (*i)->boundingBox();
	  if(planePosition <= componentBoundingBox->getMax().y())
	  {
	    leftList.push_back(*i);
	  }
	  if(planePosition >= componentBoundingBox->getMin().y())
	  {
	    rightList.push_back(*i);
	  }
	  delete componentBoundingBox;
	}
      }
    }
    else
    {
      axis = 2;
      planePosition = boundingBox->getMin().z() + diff.z() / 2;
      for(std::list< QSharedPointer<Intersectable> >::const_iterator i = components.begin(); i != components.end(); i++)
      {
	AxisAlignedBox * componentBoundingBox = (*i)->boundingBox();
	if(planePosition <= componentBoundingBox->getMax().z())
	{
	  leftList.push_back(*i);
	}
	if(planePosition >= componentBoundingBox->getMin().z())
	{
	  rightList.push_back(*i);
	}
	delete componentBoundingBox;
      }
    }
    BSPNode * leftNode = buildTree(new IntersectableList(leftList), depth + 1, maxDepth);
    BSPNode * rightNode = buildTree(new IntersectableList(rightList), depth + 1, maxDepth);
    BSPInternalNode * result = new BSPInternalNode(planePosition, axis, leftNode, rightNode, intersectables->boundingBox());
    delete boundingBox;
    delete intersectables;
    return result;
  }
}

AxisAlignedBox* BSPNode::boundingBox() const
{
  return new AxisAlignedBox(*bBox);
}

BSPLeafNode::BSPLeafNode(IntersectableList* objects): BSPNode(objects->boundingBox()), objects(objects)
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
  BSPNode *first, *second;
  double rayOrigin, rayDirection;
  QVector4D splitPlaneVector;
  switch(axis)
  {
    case 0:
      rayOrigin = ray.getOrigin().x();
      rayDirection = ray.getDirection().x();
      splitPlaneVector = QVector4D(1, 0, 0, planePosition);
      break;
    case 1:
      rayOrigin = ray.getOrigin().y();
      rayDirection = ray.getDirection().y();
      splitPlaneVector = QVector4D(0, 1, 0, planePosition);
      break;
    case 2:
      rayOrigin = ray.getOrigin().z();
      rayDirection = ray.getDirection().z();
      splitPlaneVector = QVector4D(0, 0, 1, planePosition);
      break;
  }
  
  Plane splitPlane(splitPlaneVector, QSharedPointer<Material>(new DarkMatter));
  HitRecord splitPlaneHit = splitPlane.intersect(ray, -std::numeric_limits<double>::infinity());
  double tsplit = splitPlane.intersect(ray, -std::numeric_limits<double>::infinity()).getRayParameter();
  IntersectionParameter intersection = bBox->getIntersectionParameter(ray);
  double rayTowardsFirst;
  
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
  
  if(tsplit > intersection.tmax || tsplit < 0 || (tsplit == 0 && rayTowardsFirst))
  {
    return first->intersect(ray, (from > intersection.tmin ? from : intersection.tmin), (to < intersection.tmax ? to : intersection.tmax));
  }
  else if(tsplit < intersection.tmin || (tsplit == 0 && !rayTowardsFirst))
  {
    return second->intersect(ray, (from > intersection.tmin ? from : intersection.tmin), (to < intersection.tmax ? to : intersection.tmax));
  }
  else
  {
    HitRecord firstHit = first->intersect(ray, (from > intersection.tmin ? from : intersection.tmin), tsplit);
    if(firstHit.intersects())
    {
      return firstHit;
    }
    else
    {
      return second->intersect(ray, tsplit, intersection.tmax);
    }
  }
  return HitRecord();
}
