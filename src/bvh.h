#ifndef BVH_H
#define BVH_H

#include "intersectable.h"

class IntersectableList;


class BVHNode : public Intersectable
{
public:
  BVHNode(Intersectable * left, Intersectable * right, AxisAlignedBox * bb);
  static Intersectable* create(IntersectableList * list, int maxDepth);
  virtual AxisAlignedBox* boundingBox() const;
  virtual HitRecord intersect(Ray ray, float from, float to) const;  
  
private:
  AxisAlignedBox* bb;
  Intersectable *left, *right;
};


#endif // BVH_H
