#ifndef BIDIPATHTRACINGINTEGRATOR_H
#define BIDIPATHTRACINGINTEGRATOR_H

#include "global.h"
#include "integrator.h"

class BiDiPathTracingIntegrator : public Integrator
{
public:
  cv::Vec3f integrate(const Ray &ray, const Intersectable &scene, const std::vector<const Light*> light, int recursionDepth, gsl_rng *rng) const;
};

#endif // BIDIPATHTRACINGINTEGRATOR_H
