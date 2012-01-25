#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#define EPSILON 0.0001

#include "global.h"
#include "ray.h"
#include "hitrecord.h"

#include <QtGui/QVector3D>

#include <limits>

class AxisAlignedBox;

class Intersectable
{
  public:
    virtual HitRecord intersect(Ray ray, float from = EPSILON, float to = std::numeric_limits< float >::infinity()) const = 0;
    virtual AxisAlignedBox * boundingBox() const = 0;
};

#endif // INTERSECTABLE_H
