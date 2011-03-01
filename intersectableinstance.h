#ifndef INTERSECTABLEINSTANCE_H
#define INTERSECTABLEINSTANCE_H

#include "intersectable.h"

#include <QMatrix4x4>

class IntersectableInstance : public Intersectable
{

public:
  virtual HitRecord intersect(Ray ray) const;
  IntersectableInstance(QMatrix4x4 transform, Intersectable & intersectable);
    
private:
  QMatrix4x4 transform;
  QMatrix4x4 inverseTransform;
  
  Intersectable & intersectable;
};

#endif // INTERSECTABLEINSTANCE_H
