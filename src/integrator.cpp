#include "integrator.h"

#include "spectrum.h"
#include "intersectable.h"
#include "light.h"
#include "sampler.h"

Spectrum Integrator::integrate(const Ray &ray, const Intersectable &scene, const std::vector< Light* > light, gsl_rng *rng) const
{
  int lightIndex = qrand() % light.size();
  return integrate(ray, scene, *light[lightIndex], 0, rng) * light.size();
}

