#ifndef BSP_H
#define BSP_H

#include "intersectable.h"

#include <list>
#include <QtCore/QSharedPointer>

class IntersectableList;
class Ray;

class BSPNode : public Intersectable
{
  public:
    BSPNode(AxisAlignedBox * boundingBox);
    virtual ~BSPNode();
    static BSPNode * buildTree(IntersectableList * intersectables);
    
    AxisAlignedBox * boundingBox() const;
    static int cnt;

  protected:
    AxisAlignedBox * bBox;
    
  private:
    static BSPNode * buildTree(IntersectableList * intersectables, int depth, int maxDepth, AxisAlignedBox * boundingBox);
    
};

class BSPLeafNode : public BSPNode
{
  public:
    BSPLeafNode(IntersectableList * objects, AxisAlignedBox * boundingBox);
    virtual ~BSPLeafNode();
    
    HitRecord intersect(Ray ray, float from, float to) const;
  private:
    IntersectableList * objects;
};

class BSPInternalNode : public BSPNode
{
  public:
    BSPInternalNode(float planePosition, short axis, BSPNode * lowerNode, BSPNode * upperNode, AxisAlignedBox * boundingBox);
    virtual ~BSPInternalNode();
    
    HitRecord intersect(Ray ray, float from, float to) const;
  protected:
    float planePosition;
    short axis;
    BSPNode * lowerNode, * upperNode;
};

#endif
