#ifndef UNIDIPATHTRACINGINTEGRATOR_H
#define UNIDIPATHTRACINGINTEGRATOR_H

#include "global.h"
#include "integrator.h"

#include <opencv2/core/core.hpp>

class UniDiPathTracingIntegrator: public Integrator
{
public:
  UniDiPathTracingIntegrator(float terminationProb = 0.f): terminationProb(terminationProb) {}

  cv::Vec3f integrate(const Ray& ray, const Intersectable& scene, const std::vector<const Light*> light, int recursionDepth, gsl_rng *rng) const;
  cv::Vec3f integrate(const Path& path, const Intersectable& scene, const std::vector<const Light*> light, const Sample lightSample[], long unsigned int lightIndex[]) const;

private:
  float terminationProb;
};

#endif // UNIDIPATHTRACINGINTEGRATOR_H
