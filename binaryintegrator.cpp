#include "binaryintegrator.h"

Spectrum BinaryIntegrator::integrate(const Intersectable& scene, Ray ray)
{
  HitRecord hit = scene.intersect(ray);
  if(hit.getIntersects())
  {
    return hit.getMaterial().shade(QVector3D(), QVector3D());
  }
  else
  {
    return Spectrum(0, 0, 0);
  }
}


