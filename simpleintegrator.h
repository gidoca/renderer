#ifndef SIMPLEINTEGRATOR_H
#define SIMPLEINTEGRATOR_H

#include "integrator.h"

class SimpleIntegrator : public Integrator
{
protected:
  Spectrum integrate(const Ray & ray, const Intersectable & scene, const Light & light, Sampler & sampler, int depth) const;
};

#endif // SIMPLEINTEGRATOR_H
