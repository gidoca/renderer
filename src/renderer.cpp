#include "renderer.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "jitteredsampler.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

Path Renderer::createPath(const Ray &primaryRay, const Intersectable &scene, gsl_rng *rng, Spectrum initialAlpha, float terminationProb)
{
    JitteredSampler sampler(1, 1, rng);
    Sample pathSamples[MAX_DEPTH];
    int pathLength;
    if(terminationProb > 0)
    {
      pathLength = std::max(1, std::min((int)gsl_ran_negative_binomial(rng, terminationProb, 1), (int)MAX_DEPTH));
    }
    else
    {
      terminationProb = 1;
      pathLength = MAX_DEPTH;
    }
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

    assert(!isnan(alpha.x()) && !isnan(alpha.y()) && !isnan(alpha.z()));
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
      if(pdf == 0) return result;
      Spectrum brdf = hit.getMaterial().shade(hit, -outDirection);
      float cos = QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized());
      assert(cos >= 0 && !isnan(pdf));
      assert(pdf >= 0 && !isnan(pdf));
      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      assert(!isnan(brdf.x()) && !isnan(brdf.y()) && !isnan(brdf.z()));
      alpha *= brdf * cos / pdf / russianRoulettePdf;
    }
    hit = scene.intersect(Ray(hit.getIntersectingPoint(), outDirection));
  }
  return result;
}

boost::program_options::options_description Renderer::options() const
{
  return boost::program_options::options_description("Renderer");
}
