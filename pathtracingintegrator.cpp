#include "pathtracingintegrator.h"

#include "hitrecord.h"
#include "sampler.h"
#include "intersectable.h"
#include "light.h"

#include <list>

Spectrum PathTracingIntegrator::integrate(const Ray& ray, const Intersectable& scene, const Light& light, Sampler& sampler, int recursionDepth) const
{
  std::list<Sample> samples = sampler.getSamples();
  Spectrum color;
  HitRecord firstHit = scene.intersect(ray);
  for(std::list<Sample>::iterator i = samples.begin(); i != samples.end(); i++)
  {
    Ray currentRay = ray;
    Spectrum alpha(1, 1, 1);
    HitRecord hit = firstHit;
    for(int k = 0; k < 3; k++)
    {
      QVector3D direction;
      Spectrum lightIntensity = light.getIntensity(hit, direction, scene, *i);
      color += alpha * hit.getMaterial().shade(hit, direction) * lightIntensity;
//      if(qrand() < RAND_MAX / 3) break;

      double pdf;
      QVector3D outDirection = i->getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
      Spectrum brdf = hit.getMaterial().shade(hit, -outDirection);
      alpha *= brdf * QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized()) / pdf /* / 0.66 */;
      hit = scene.intersect(currentRay);
      currentRay = Ray(hit.getIntersectingPoint(), outDirection);
    }
  }
  return color / samples.size();
}

