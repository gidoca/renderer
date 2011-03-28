#include "bsp.h"
#include "intersectablelist.h"

#include <cmath>

BSPNode * BSPNode::buildTree(IntersectableList * intersectables)
{
  return buildTree(intersectables->getComponents(), 0, 8 + (int) (1.3 * log(intersectables->getComponents().size())));
}

BSPNode* BSPNode::buildTree(std::list< QSharedPointer< Intersectable > > intersectables, int depth, int maxDepth)
{
  if(depth >= maxDepth)
  {
    BSPLeafNode *result = new BSPLeafNode();
    result->objects = intersectables;
    return result;
  }
  
  return 0;
}
