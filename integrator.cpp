#include "integrator.h"

#include "spectrum.h"
#include "intersectable.h"
#include "light.h"
#include "sampler.h"

Spectrum Integrator::integrate(const Ray &ray, const Intersectable &scene, const std::list< QSharedPointer<Light> > light, Sampler & sampler) const
{
  Spectrum result;
  for(std::list<QSharedPointer<Light> >::const_iterator lights = light.begin(); lights != light.end(); lights++)
  {
    result += integrate(ray, scene, **lights, sampler, 0);
  }
  return result;
}

