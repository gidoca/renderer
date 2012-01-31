#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "global.h"
#include "spectrum.h"

#include <gsl/gsl_rng.h>

#include <vector>

class Integrator
{
public:
  Spectrum integrate(const Ray & ray, const Intersectable & scene, std::vector< Light* > light, gsl_rng *rng) const;

protected:
  virtual Spectrum integrate(const Ray &ray, const Intersectable &scene, const Light & light, int recursionDepth, gsl_rng *rng) const = 0;
};

#endif // INTEGRATOR_H
