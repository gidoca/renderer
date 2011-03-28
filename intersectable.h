#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#define EPSILON 0.00005

#include "ray.h"
#include "hitrecord.h"

#include <QVector3D>

#include <limits>

class AxisAlignedBox;

class Intersectable
{
  public:
    virtual HitRecord intersect(Ray ray, double from = EPSILON, double to = std::numeric_limits< double >::infinity()) const = 0;
    virtual AxisAlignedBox * boundingBox() const = 0;
};

#endif // INTERSECTABLE_H
