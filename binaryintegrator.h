#ifndef BINARYINTEGRATOR_H
#define BINARYINTEGRATOR_H

#include "integrator.h"

class BinaryIntegrator : public Integrator
{
  public:
    virtual Spectrum integrate(const Intersectable & scene, Ray ray, Light & light);
};

#endif // BINARYINTEGRATOR_H
