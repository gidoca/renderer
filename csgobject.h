#ifndef CSGOBJECT_H
#define CSGOBJECT_H

#include "intersectable.h"

class IntersectionParameter
{
  public:
    std::list<double> intersections;
    QVector3D normal;
    QSharedPointer<Material> material;
};

class CSGObject : public Intersectable
{
public:
  HitRecord intersect(Ray ray, double from = 0.00005, double to = std::numeric_limits< double >::infinity()) const;
  virtual IntersectionParameter getCSGIntersection(Ray ray) const = 0;
};

#endif // CSGOBJECT_H
