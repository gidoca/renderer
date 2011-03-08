#include "binaryintegrator.h"

#include "light.h"

Spectrum BinaryIntegrator::integrate(const Intersectable& scene, Ray ray, Light & light)
{
  HitRecord hit = scene.intersect(ray);
  if(light.isOccluded(hit.getIntersectingPoint(), scene))
  {
    return Spectrum();
  }
  else
  {
    return hit.getMaterial().shade(hit, light);
  }
}


