#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "ray.h"

#include <QVector3D>

class Intersectable
{
  virtual const bool intersect(Ray ray) = 0;
};

#endif // INTERSECTABLE_H
