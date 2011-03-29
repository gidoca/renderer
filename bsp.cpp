#include "bsp.h"
#include "intersectablelist.h"

#include <cmath>

BSPNode * BSPNode::buildTree(IntersectableList * intersectables)
{
  return buildTree(intersectables, 0, 8 + (int) (1.3 * log(intersectables->getComponents().size())));
}

BSPNode* BSPNode::buildTree(IntersectableList * intersectables, int depth, int maxDepth)
{
  if(depth >= maxDepth)
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
    if(diff.x() > diff.z())
    {
      if(diff.x() > diff.y())
      {
	axis = 0;
	planePosition = boundingBox->getMin().x() + diff.x() / 2;
	for(std::list< QSharedPointer<Intersectable> >::const_iterator i = intersectables->getComponents().begin(); i != intersectables->getComponents().end(); i++)
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
 	for(std::list< QSharedPointer<Intersectable> >::const_iterator i = intersectables->getComponents().begin(); i != intersectables->getComponents().end(); i++)
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
      for(std::list< QSharedPointer<Intersectable> >::const_iterator i = intersectables->getComponents().begin(); i != intersectables->getComponents().end(); i++)
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
    delete boundingBox;
    delete intersectables;
    BSPNode * leftNode = buildTree(new IntersectableList(leftList), depth + 1, maxDepth);
    BSPNode * rightNode = buildTree(new IntersectableList(rightList), depth + 1, maxDepth);
    return  new BSPInternalNode(planePosition, axis, leftNode, rightNode);
  }
}

BSPLeafNode::BSPLeafNode(IntersectableList* objects): objects(objects)
{

}

BSPInternalNode::BSPInternalNode(double planePosition, short int axis, BSPNode* lowerNode, BSPNode* upperNode): planePosition(planePosition), axis(axis), lowerNode(lowerNode), upperNode(upperNode)
{

}

BSPInternalNode::~BSPInternalNode()
{
  delete lowerNode;
  delete upperNode;
}
