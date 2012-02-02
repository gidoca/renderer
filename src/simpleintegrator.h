#ifndef SIMPLEINTEGRATOR_H
#define SIMPLEINTEGRATOR_H

#include "global.h"
#include "integrator.h"

class SimpleIntegrator : public Integrator
{
protected:
  Spectrum integrate(const Ray & ray, const Intersectable & scene, const Light & light, int depth, gsl_rng *rng) const;
};

#endif // SIMPLEINTEGRATOR_H