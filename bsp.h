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
    BSPNode(AxisAlignedBox * boundingBox);
    virtual ~BSPNode();
    static BSPNode * buildTree(IntersectableList * intersectables);
    
    AxisAlignedBox * boundingBox() const;
    
  protected:
    AxisAlignedBox * bBox;
    
  private:
    static BSPNode * buildTree(IntersectableList * intersectables, int depth, int maxDepth);
    
};

class BSPLeafNode : public BSPNode
{
  public:
    BSPLeafNode(IntersectableList * objects);
    
    HitRecord intersect(Ray ray, double from, double to) const;
  private:
    IntersectableList * objects;
};

class BSPInternalNode : public BSPNode
{
  public:
    BSPInternalNode(double planePosition, short axis, BSPNode * lowerNode, BSPNode * upperNode, AxisAlignedBox * boundingBox);
    virtual ~BSPInternalNode();
    
    HitRecord intersect(Ray ray, double from, double to) const;
  protected:
    double planePosition;
    short axis;
    BSPNode * lowerNode, * upperNode;
};

#endif