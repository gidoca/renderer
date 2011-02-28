#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "intersectable.h"
#include "ray.h"

#include <qglobal.h>

class Integrator
{
  public:
    virtual uint integrate(const Intersectable & scene, Ray ray) = 0;
};

#endif // INTEGRATOR_H
