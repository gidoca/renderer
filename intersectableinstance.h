#ifndef INTERSECTABLEINSTANCE_H
#define INTERSECTABLEINSTANCE_H

#include "intersectable.h"

#include <QMatrix4x4>
#include <QSharedPointer>

class IntersectableInstance : public Intersectable
{

public:
  IntersectableInstance(QMatrix4x4 transform, QSharedPointer<Intersectable> intersectable) : transform(transform), inverseTransform(transform.inverted()), intersectable(intersectable)
  {

  }

  HitRecord intersect(Ray ray, double from, double to) const;
  AxisAlignedBox * boundingBox() const;

private:
  QMatrix4x4 transform;
  QMatrix4x4 inverseTransform;
  
  QSharedPointer<Intersectable> intersectable;
};

#endif // INTERSECTABLEINSTANCE_H
