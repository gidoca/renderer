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

Path PathTracingIntegrator::createPath(const Ray& primaryRay, const Intersectable &scene, Spectrum alpha) const
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
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
