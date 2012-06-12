#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "global.h"

#include <gsl/gsl_rng.h>

#include <opencv2/core/core.hpp>

#include <vector>

class Integrator
{
public:
  inline cv::Vec3f integrate(const Ray & ray, const Intersectable & scene, const std::vector<const Light* > light, gsl_rng *rng) const
  {
    return integrate(ray, scene, light, 0, rng);
  }
protected:
  virtual cv::Vec3f integrate(const Ray & ray, const Intersectable & scene, const std::vector< const Light* > light, int recursionDepth, gsl_rng *rng) const = 0;

};

#endif // INTEGRATOR_H
