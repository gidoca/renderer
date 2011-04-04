#include "integrator.h"

#include "spectrum.h"
#include "intersectable.h"
#include "light.h"

Spectrum Integrator::integrate(const Ray &ray, const Intersectable &scene, const Light &light) const
{
  return integrate(ray, scene, light, 0);
}

Spectrum Integrator::integrate(const Ray & ray, const Intersectable & scene, const Light & light, int depth) const
{
  if(depth > MAX_DEPTH) return Spectrum();

  HitRecord hit = scene.intersect(ray);
  if(hit.getMaterial().isMirror())
  {
    QVector3D oldRayDirection = hit.getRay().getDirection().toVector3D();
    QVector3D surfaceNormal = hit.getSurfaceNormal();
    surfaceNormal.normalize();
    QVector3D newDirection = oldRayDirection - 2 * QVector3D::dotProduct(oldRayDirection, surfaceNormal) * surfaceNormal;
    Ray mirrorRay(hit.getIntersectingPoint(), newDirection);
    return integrate(mirrorRay, scene, light, depth + 1);
  }
  else
  {
    if(light.isOccluded(hit.getIntersectingPoint(), scene))
    {
      return Spectrum();
    }
    else
    {
      return hit.getMaterial().shade(hit, light);
    }
  }
}