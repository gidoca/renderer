#ifndef UNIDIPATHTRACINGINTEGRATOR_H
#define UNIDIPATHTRACINGINTEGRATOR_H

#include "global.h"
#include "pathtracingintegrator.h"

class UniDiPathTracingIntegrator: public PathTracingIntegrator
{
public:
    virtual Spectrum integrate(const Ray& ray, const Intersectable& scene, const Light& light, int recursionDepth, gsl_rng *rng) const;
};

#endif // UNIDIPATHTRACINGINTEGRATOR_H
