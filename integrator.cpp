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
    if(hit.intersects())
    {
    Spectrum result;
    QVector3D direction;
    std::list<QPointF> samples = sampler.getSamples();
    for(std::list<QPointF>::iterator i = samples.begin(); i != samples.end(); i++)
    {
      Spectrum lightIntensity = light.getIntensity(hit, direction, scene, *i);
      Spectrum shade = hit.getMaterial().shade(hit, direction);
      result += QVector3D(shade.x() * lightIntensity.x(), shade.y() * lightIntensity.y(), shade.z() * lightIntensity.z());
    }
    return result / samples.size();
  }
    else
    {
      /*QVector3D dir = ray.getDirection().toVector3D();
      return light.getIntensity(hit, dir, scene, QPointF());*/
      return Spectrum();
    }
  }
}
