#include "binaryintegrator.h"

Spectrum BinaryIntegrator::integrate(const Intersectable& scene, Ray ray, Light & light)
{
  HitRecord hit = scene.intersect(ray);
  return hit.getMaterial().shade(hit, light);
}


