#include "binaryintegrator.h"

Spectrum BinaryIntegrator::integrate(const Intersectable& scene, Ray ray)
{
  if(scene.intersect(ray).getIntersects())
  {
    return Spectrum(1, 1, 1);
  }
  else
  {
    return Spectrum(0, 0, 0);
  }
}


