#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "ray.h"

#include <QVector3D>

class Intersectable
{
  virtual bool intersect(Ray ray) const = 0;
};

#endif // INTERSECTABLE_H
