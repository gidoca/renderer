#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "spectrum.h"

#include <list>
#include <QSharedPointer>

#define MAX_DEPTH 6

class Ray;
class Intersectable;
class Light;
class Sampler;

class Integrator
{
public:
  Spectrum integrate(const Ray & ray, const Intersectable & scene, std::list< QSharedPointer<Light> > light, Sampler & sampler) const;

protected:
  virtual Spectrum integrate(const Ray &ray, const Intersectable &scene, const Light & light, Sampler & sampler, int recursionDepth) const = 0;
};

#endif // INTEGRATOR_H
