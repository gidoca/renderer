#ifndef SIMPLEINTEGRATOR_H
#define SIMPLEINTEGRATOR_H

#include "global.h"
#include "integrator.h"

class SimpleIntegrator : public Integrator
{
protected:
  cv::Vec3f integrate(const Ray & ray, const Intersectable & scene, std::vector<Light *> light, int depth, gsl_rng *rng) const;
};

#endif // SIMPLEINTEGRATOR_H
