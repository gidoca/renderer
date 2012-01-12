#include "simpleintegrator.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "jitteredsampler.h"
#include "light.h"

Spectrum SimpleIntegrator::integrate(const Ray & ray, const Intersectable & scene, const Light & light, int depth) const
{
  JitteredSampler sampler(2, 2);
  if(depth > MAX_DEPTH) return Spectrum();

  HitRecord hit = scene.intersect(ray);
  if(hit.getMaterial().isMirror())
  {
    QVector3D oldRayDirection = hit.getRay().getDirection();
    QVector3D surfaceNormal = hit.getSurfaceNormal();
    surfaceNormal.normalize();
    QVector3D newDirection = oldRayDirection - 2 * QVector3D::dotProduct(oldRayDirection, surfaceNormal) * surfaceNormal;
    Ray mirrorRay(hit.getIntersectingPoint(), newDirection);
    return integrate(mirrorRay, scene, light, depth + 1);
  }
  else
  {
    if(hit.intersects())
    {
    Spectrum result;
    QVector3D direction;
    std::list<Sample> samples = sampler.getSamples();
    for(std::list<Sample>::iterator i = samples.begin(); i != samples.end(); i++)
    {
      Spectrum lightIntensity = light.getIntensity(hit.getIntersectingPoint(), direction, scene, *i);
      Spectrum shade = hit.getMaterial().shade(hit, direction);
      result += shade * lightIntensity;
    }
    return result / samples.size();
  }
    else
    {
//      QVector3D dir = ray.getDirection();
//      return light.getIntensity(hit, dir, scene, QPointF());
      return Spectrum();
    }
  }
}
