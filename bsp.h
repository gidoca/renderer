#ifndef BSP_H
#define BSP_H

#include "intersectable.h"

#include <list>
#include <QSharedPointer>

class IntersectableList;
class Ray;

class BSPNode : public Intersectable
{
  public:
    static BSPNode * buildTree(IntersectableList * intersectables);
    
    HitRecord intersect(Ray ray, double from, double to) const;
    AxisAlignedBox * boundingBox() const;
    
  private:
    static BSPNode * buildTree(IntersectableList * intersectables, int depth, int maxDepth);
};

class BSPLeafNode : public BSPNode
{
  public:
    BSPLeafNode(IntersectableList * objects);
  private:
    IntersectableList * objects;
};

class BSPInternalNode : public BSPNode
{
  public:
    BSPInternalNode(double planePosition, short axis, BSPNode * lowerNode, BSPNode * upperNode);
    virtual ~BSPInternalNode();
  protected:
    double planePosition;
    short axis;
    BSPNode * lowerNode, * upperNode;
};

#endif