#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "intersectable.h"
#include "ray.h"
#include "spectrum.h"

#include <qglobal.h>

class Integrator
{
  public:
    virtual Spectrum integrate(const Intersectable & scene, Ray ray, Light & light) = 0;
};

#endif // INTEGRATOR_H
