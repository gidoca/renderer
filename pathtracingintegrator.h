#ifndef PATHTRACINGINTEGRATOR_H
#define PATHTRACINGINTEGRATOR_H

#include "integrator.h"

class PathTracingIntegrator : public Integrator
{

protected:
  virtual Spectrum integrate(const Ray& ray, const Intersectable& scene, const Light& light, int recursionDepth) const;
};

#endif // PATHTRACINGINTEGRATOR_H
