#ifndef BSP_H
#define BSP_H

#include <list>
#include <QSharedPointer>

class Intersectable;
class IntersectableList;

class BSPNode
{
  public:
    static BSPNode * buildTree(IntersectableList * intersectables);
    
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