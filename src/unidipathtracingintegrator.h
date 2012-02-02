#ifndef UNIDIPATHTRACINGINTEGRATOR_H
#define UNIDIPATHTRACINGINTEGRATOR_H

#include "global.h"
#include "pathtracingintegrator.h"

class UniDiPathTracingIntegrator: public PathTracingIntegrator
{
public:
  Spectrum integrate(const Ray& ray, const Intersectable& scene, const Light& light, int recursionDepth, gsl_rng *rng) const;
  Spectrum integrate(const Path& path, const Intersectable& scene, const Light& light, const Sample lightSample[]) const;
};

#endif // UNIDIPATHTRACINGINTEGRATOR_H
