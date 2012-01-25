#ifndef BIDIPATHTRACINGINTEGRATOR_H
#define BIDIPATHTRACINGINTEGRATOR_H

#include "global.h"
#include "pathtracingintegrator.h"

class BiDiPathTracingIntegrator : public PathTracingIntegrator
{
public:
  Spectrum integrate(const Ray &ray, const Intersectable &scene, const Light &light, int recursionDepth) const;
};

#endif // BIDIPATHTRACINGINTEGRATOR_H
