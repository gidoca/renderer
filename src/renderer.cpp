#include "renderer.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "jitteredsampler.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <cmath>
#include <cassert>
#include <algorithm>

//#define RUSSIAN_ROULETTE

Path Renderer::createPath(const Ray &primaryRay, const Intersectable &scene, gsl_rng *rng, Spectrum initialAlpha)
{
    JitteredSampler sampler(1, 1, rng);
    Sample pathSamples[MAX_DEPTH];
#ifdef RUSSIAN_ROULETTE
    const float terminationProb = 0.5;
    const int pathLength = std::max(1, std::min((int)gsl_ran_negative_binomial(rng, terminationProb, 1), (int)MAX_DEPTH));
#else
    const float terminationProb = 1;
    const int pathLength = MAX_DEPTH;
#endif
    for(int i = 0; i < pathLength; i++)
    {
      pathSamples[i] = sampler.getSamples().front();
    }
    return createPath(primaryRay, scene, pathSamples, initialAlpha, pathLength, terminationProb);
}

Path Renderer::createPath(const Ray& primaryRay, const Intersectable &scene, Sample pathSamples[], Spectrum alpha, int pathLength, float russianRoulettePdf)
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
  for(int i = 0; i < pathLength; i++)
  {
    if(!hit.intersects()) return result;

    result.alphaValues.push_back(alpha);
    result.hitRecords.push_back(hit);

    float pdf;
    QVector3D outDirection;
    if(hit.getMaterial().isMirror())
    {
      QVector3D oldRayDirection = hit.getRay().getDirection();
      QVector3D surfaceNormal = hit.getSurfaceNormal();
      surfaceNormal.normalize();
      outDirection = oldRayDirection - 2 * QVector3D::dotProduct(oldRayDirection, surfaceNormal) * surfaceNormal;
    }
    else
    {
      outDirection = pathSamples[i].getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
      Spectrum brdf = hit.getMaterial().shade(hit, -outDirection);
      float cos = QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized());
      assert(cos >= 0);
      assert(pdf >= 0);
      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      alpha *= brdf * cos / pdf / russianRoulettePdf;
    }
    hit = scene.intersect(Ray(hit.getIntersectingPoint(), outDirection));
  }
  return result;
}

