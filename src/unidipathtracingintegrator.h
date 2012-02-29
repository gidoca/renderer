#ifndef UNIDIPATHTRACINGINTEGRATOR_H
#define UNIDIPATHTRACINGINTEGRATOR_H

#include "global.h"
#include "pathtracingintegrator.h"

class UniDiPathTracingIntegrator: public PathTracingIntegrator
{
public:
  UniDiPathTracingIntegrator(float terminationProb = 0.f): terminationProb(terminationProb) {}

  Spectrum integrate(const Ray& ray, const Intersectable& scene, std::vector<Light*> light, int recursionDepth, gsl_rng *rng) const;
  Spectrum integrate(const Path& path, const Intersectable& scene, std::vector<Light*> light, const Sample lightSample[], long unsigned int lightIndex[]) const;

private:
  float terminationProb;
};

#endif // UNIDIPATHTRACINGINTEGRATOR_H
