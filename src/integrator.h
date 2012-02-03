#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "global.h"
#include "spectrum.h"

#include <gsl/gsl_rng.h>

#include <vector>

class Integrator
{
public:
  inline Spectrum integrate(const Ray & ray, const Intersectable & scene, std::vector< Light* > light, gsl_rng *rng) const
  {
    return integrate(ray, scene, light, 0, rng);
  }
  virtual Spectrum integrate(const Ray & ray, const Intersectable & scene, std::vector< Light* > light, int recursionDepth, gsl_rng *rng) const;

};

#endif // INTEGRATOR_H
