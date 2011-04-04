#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "spectrum.h"

#define MAX_DEPTH 6

class Ray;
class Intersectable;
class Light;

class Integrator
{
public:
    Spectrum integrate(const Ray & ray, const Intersectable & scene, const Light & light) const;

private:
    Spectrum integrate(const Ray &ray, const Intersectable &scene, const Light & light, int recursionDepth) const;
};

#endif // INTEGRATOR_H