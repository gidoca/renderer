#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "global.h"
#include "spectrum.h"

#include <gsl/gsl_rng.h>

#include <vector>

class Integrator
{
public:
  inline Spectrum integrate(const Ray & ray, const Intersectable & scene, const std::vector<const Light* > light, gsl_rng *rng) const
  {
    return integrate(ray, scene, light, 0, rng);
  }
protected:
  virtual Spectrum integrate(const Ray & ray, const Intersectable & scene, const std::vector< const Light* > light, int recursionDepth, gsl_rng *rng) const = 0;

};

#endif // INTEGRATOR_H
