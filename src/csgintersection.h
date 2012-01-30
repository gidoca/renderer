#ifndef CSGINTERSECTION_H
#define CSGINTERSECTION_H

#include "global.h"
#include "csgobject.h"

class CSGIntersection : public CSGObject
{
public:
  CSGIntersection(CSGObject* left, CSGObject* right) : left(left), right(right) {}

  IntersectionParameter getCSGIntersection(Ray ray) const;
  AxisAlignedBox * boundingBox() const;

private:
  CSGObject *left, *right;
};

#endif // CSGINTERSECTION_H
