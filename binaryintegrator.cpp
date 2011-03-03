#include "binaryintegrator.h"

Spectrum BinaryIntegrator::integrate(const Intersectable& scene, Ray ray)
{
  HitRecord hit = scene.intersect(ray);
  return hit.getMaterial().shade(QVector3D(), QVector3D());
}


