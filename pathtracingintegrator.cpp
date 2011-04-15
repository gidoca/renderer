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
  for(std::list<Sample>::iterator i = samples.begin(); i != samples.end(); i++)
  {
    Ray currentRay = ray;
    Spectrum alpha(1, 1, 1);
    HitRecord hit = scene.intersect(currentRay);
    for(int k = 0; k < 10; k++)
    {
      QVector3D direction;
      Spectrum lightIntensity = light.getIntensity(hit, direction, scene, *i);
      color += alpha * hit.getMaterial().shade(hit, direction) * lightIntensity;
      if(qrand() < RAND_MAX / 3) break;

      hit = scene.intersect(currentRay);
      QVector3D outDirection = i->getCosineWeightedDirection(hit.getSurfaceNormal());
      alpha *= hit.getMaterial().shade(hit, -outDirection);
      currentRay = Ray(hit.getIntersectingPoint(), outDirection);
    }
  }
  return color / samples.size();
}

