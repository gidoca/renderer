#ifndef INTERSECTABLEINSTANCE_H
#define INTERSECTABLEINSTANCE_H

#include "global.h"
#include "intersectable.h"

#include <QtGui/QMatrix4x4>

class IntersectableInstance : public Intersectable
{

public:
  IntersectableInstance(QMatrix4x4 transform, Intersectable* intersectable) : transform(transform), inverseTransform(transform.inverted()), intersectable(intersectable)
  {

  }

  HitRecord intersect(Ray ray, float from, float to) const;
  AxisAlignedBox * boundingBox() const;

private:
  QMatrix4x4 transform;
  QMatrix4x4 inverseTransform;
  
  Intersectable* intersectable;
};

#endif // INTERSECTABLEINSTANCE_H
