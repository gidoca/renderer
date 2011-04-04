#include "integrator.h"

#include "spectrum.h"
#include "intersectable.h"
#include "light.h"
#include "sampler.h"

Spectrum Integrator::integrate(const Ray &ray, const Intersectable &scene, std::list< QSharedPointer<Light> > light, Sampler & sampler) const
{
  Spectrum result;
  for(std::list<QSharedPointer<Light> >::const_iterator lights = light.begin(); lights != light.end(); lights++)
  {
    result += integrate(ray, scene, **lights, sampler, 0);
  }
  return result;
}

Spectrum Integrator::integrate(const Ray & ray, const Intersectable & scene, const Light & light, Sampler & sampler, int depth) const
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
    return integrate(mirrorRay, scene, light, sampler, depth + 1);
  }
  else
  {
    Spectrum result;
    std::list<QPointF> samples = sampler.getSamples();
    for(std::list<QPointF>::iterator i = samples.begin(); i != samples.end(); i++)
    {
      if(!light.isOccluded(hit.getIntersectingPoint(), scene))
      {
        result += hit.getMaterial().shade(hit, light);
      }
    }
    return result / samples.size();
  }
}
