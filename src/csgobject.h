#ifndef CSGOBJECT_H
#define CSGOBJECT_H

#include "global.h"
#include "intersectable.h"

class IntersectionParameter
{
  public:
    std::list<float> intersections;
    QVector3D normal;
    QSharedPointer<Material> material;
};

class CSGObject : public Intersectable
{
public:
  HitRecord intersect(Ray ray, float from = 0.00005, float to = std::numeric_limits< float >::infinity()) const;
  virtual IntersectionParameter getCSGIntersection(Ray ray) const = 0;
};

#endif // CSGOBJECT_H
