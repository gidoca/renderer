#include "pathtracingintegrator.h"

#include "hitrecord.h"
#include "sampler.h"
#include "intersectable.h"
#include "light.h"

#include <list>
#include <iostream>
#include <cmath>
#include <cassert>

Spectrum PathTracingIntegrator::integrate(const Ray& ray, const Intersectable& scene, const Light& light, Sampler& sampler, int recursionDepth) const
{
  std::list<Sample> samples = sampler.getSamples();
  Spectrum color;
  HitRecord firstHit = scene.intersect(ray);
  if(!firstHit.intersects()) return Spectrum();
  for(std::list<Sample>::iterator i = samples.begin(); i != samples.end(); i++)
  {
    Ray currentRay = ray;
    Spectrum alpha(1, 1, 1);
    HitRecord hit = firstHit;
    for(int k = 0; k < 5; k++)
    {
      QVector3D direction;
      Spectrum lightIntensity = light.getIntensity(hit, direction, scene, *i);
      double inCos = QVector3D::dotProduct(-direction.normalized(), hit.getSurfaceNormal().normalized());
      Spectrum brdf;
      if(inCos > 0)
      {
        assert(!isnan(lightIntensity.x()) && !isnan(lightIntensity.y()) && !isnan(lightIntensity.z()));
        brdf = hit.getMaterial().shade(hit, direction);
        assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
        color += alpha * brdf * lightIntensity * inCos;
      }
      if(k >= 2 && qrand() < RAND_MAX / 2) break;

      double pdf;
      QVector3D outDirection = i->getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
      brdf = hit.getMaterial().shade(hit, -outDirection);
      double cos = QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized());
//      std::cout << cos << std::endl;
      assert(cos >= 0);
      assert(pdf >= 0);
      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      double russianRoulettePdf = (k < 2 ? 1 : .5);
      alpha *= brdf * cos / pdf / russianRoulettePdf;
      currentRay = Ray(hit.getIntersectingPoint(), outDirection);
      hit = scene.intersect(currentRay);
      if(!hit.intersects()) break;
    }
  }
  return color / samples.size();
}

