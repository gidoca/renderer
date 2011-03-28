#ifndef BSP_H
#define BSP_H

#include <list>
#include <QSharedPointer>

class Intersectable;
class IntersectableList;

class BSPNode
{
  public:
    virtual ~BSPNode();
    
    static BSPNode * buildTree(IntersectableList * intersectables);
    
  private:
    static BSPNode * buildTree(std::list< QSharedPointer< Intersectable > > intersectables, int depth, int maxDepth);
};

class BSPLeafNode : public BSPNode
{
  protected:
    std::list< QSharedPointer<Intersectable> > objects;
};

class BSPInternalNode : public BSPNode
{
  public:
    virtual ~BSPInternalNode();
  protected:
    double planePosition;
    char axis;
    BSPNode * lowerNode, * upperNode;
};

#endif