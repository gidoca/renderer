#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "ray.h"
#include "hitrecord.h"
#include "light.h"

#include <QVector3D>

class Intersectable
{
  public:
    virtual HitRecord intersect(Ray ray) const = 0;
};

#endif // INTERSECTABLE_H
