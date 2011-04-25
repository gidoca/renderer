#ifndef PATHTRACINGINTEGRATOR_H
#define PATHTRACINGINTEGRATOR_H

#define ROUSSIAN_ROULETTE

#include "integrator.h"

class Path;

class PathTracingIntegrator : public Integrator
{

protected:
  virtual Spectrum integrate(const Ray& ray, const Intersectable& scene, const Light& light, int recursionDepth) const;
  Path createPath(const Ray& primaryRay, const Intersectable& scene) const;
};

#endif // PATHTRACINGINTEGRATOR_H
