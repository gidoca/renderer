#ifndef BINARYINTEGRATOR_H
#define BINARYINTEGRATOR_H

#include "integrator.h"

class BinaryIntegrator : public Integrator
{
  public:
    virtual uint integrate(const Intersectable & scene, Ray ray);
};

#endif // BINARYINTEGRATOR_H
