#ifndef PATHTRACINGINTEGRATOR_H
#define PATHTRACINGINTEGRATOR_H

#define ROUSSIAN_ROULETTE

#include "integrator.h"

class Path;

class PathTracingIntegrator : public Integrator
{

protected:
  Path createPath(const Ray& primaryRay, const Intersectable& scene) const;
};

#endif // PATHTRACINGINTEGRATOR_H
