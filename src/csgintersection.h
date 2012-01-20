#ifndef CSGINTERSECTION_H
#define CSGINTERSECTION_H

#include <QtCore/QSharedPointer>

#include "csgobject.h"

class CSGIntersection : public CSGObject
{
public:
  CSGIntersection(QSharedPointer<CSGObject> left, QSharedPointer<CSGObject> right) : left(left), right(right) {}

  IntersectionParameter getCSGIntersection(Ray ray) const;
  AxisAlignedBox * boundingBox() const;

private:
  QSharedPointer<CSGObject> left, right;
};

#endif // CSGINTERSECTION_H
