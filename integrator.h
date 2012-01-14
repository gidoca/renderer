#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "spectrum.h"

#include <list>
#include <QtCore/QSharedPointer>

#define MAX_DEPTH 4

class Ray;
class Intersectable;
class Light;
class Sampler;

class Integrator
{
public:
  Spectrum integrate(const Ray & ray, const Intersectable & scene, std::list< QSharedPointer<Light> > light) const;

protected:
  virtual Spectrum integrate(const Ray &ray, const Intersectable &scene, const Light & light, int recursionDepth) const = 0;
};

#endif // INTEGRATOR_H
