#ifndef PATHTRACINGINTEGRATOR_H
#define PATHTRACINGINTEGRATOR_H

#define ROUSSIAN_ROULETTE

#include "global.h"
#include "integrator.h"
#include "spectrum.h"

class PathTracingIntegrator : public Integrator
{

protected:
  Path createPath(const Ray& primaryRay, const Intersectable& scene, Spectrum initialAlpha = Spectrum(1, 1, 1)) const;
};

#endif // PATHTRACINGINTEGRATOR_H
