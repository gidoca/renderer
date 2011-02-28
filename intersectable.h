#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "ray.h"
#include "hitrecord.h"

#include <QVector3D>

class Intersectable
{
  virtual HitRecord intersect(Ray ray) const = 0;
};

#endif // INTERSECTABLE_H
