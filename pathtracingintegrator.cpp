#include "pathtracingintegrator.h"

#include "hitrecord.h"
#include "jitteredsampler.h"
#include "intersectable.h"
#include "light.h"
#include "path.h"

#include <list>
#include <iostream>
#include <cmath>
#include <cassert>

Spectrum PathTracingIntegrator::integrate(const Ray& ray, const Intersectable& scene, const Light& light, int recursionDepth) const
{
  JitteredSampler sampler(1, 1);
  Spectrum color;
  Path path = createPath(ray, scene);

  std::list<Spectrum>::iterator alphaIt = path.alphaValues.begin();
  std::list<HitRecord>::iterator hitIt = path.hitRecords.begin();

  while(alphaIt != path.alphaValues.end() && hitIt != path.hitRecords.end())
  {
    QVector3D direction;
    Spectrum lightIntensity = light.getIntensity(*hitIt, direction, scene, sampler.getSamples().front());
    double inCos = QVector3D::dotProduct(-direction.normalized(), hitIt->getSurfaceNormal().normalized());
    Spectrum brdf;
    if(inCos > 0)
    {
      assert(!isnan(lightIntensity.x()) && !isnan(lightIntensity.y()) && !isnan(lightIntensity.z()));
      brdf = hitIt->getMaterial().shade(*hitIt, direction);
      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      color += *alphaIt * brdf * lightIntensity * inCos;
    }

    alphaIt++;
    hitIt++;
  }

  return color;
}

Path PathTracingIntegrator::createPath(const Ray& primaryRay, const Intersectable &scene) const
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
  Spectrum alpha(1, 1, 1);
  JitteredSampler sampler(1, 1);
  for(int i = 0; i < MAX_DEPTH; i++)
  {
    if(!hit.intersects()) return result;

#ifdef ROUSSIAN_ROULETTE
    const double terminationProb = 0.7;
    if(i >= 2 && qrand() < RAND_MAX * terminationProb) return result;
    double russianRoulettePdf = (i < 2 ? 1 : 1 - terminationProb);
#else
    const double russianRoulettePdf = 1;
#endif

    result.alphaValues.push_back(alpha);
    result.hitRecords.push_back(hit);

    double pdf;
    QVector3D outDirection = sampler.getSamples().front().getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
    Spectrum brdf = hit.getMaterial().shade(hit, -outDirection);
    double cos = QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized());
    assert(cos >= 0);
    assert(pdf >= 0);
    assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
    alpha *= brdf * cos / pdf / russianRoulettePdf;
    hit = scene.intersect(Ray(hit.getIntersectingPoint(), outDirection));
  }
  return result;
}
