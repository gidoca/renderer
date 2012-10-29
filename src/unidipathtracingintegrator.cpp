#include "unidipathtracingintegrator.h"

#include "jitteredsampler.h"
#include "path.h"
#include "light.h"
#include "hitrecord.h"
#include "renderer.h"

#include <cmath>
#include <cassert>

using namespace cv;

Vec3f UniDiPathTracingIntegrator::integrate(const Ray& ray, const Intersectable& scene, const std::vector<const Light*> light, gsl_rng *rng) const
{
  JitteredSampler sampler(1, 1, rng);
  Path path = Renderer::createPath(ray, scene, rng, Vec3f(1, 1, 1), terminationProb);
  Sample lightSamples[MAX_DEPTH];
  long unsigned int lightIndex[MAX_DEPTH];
  for(int i = 0; i < MAX_DEPTH; i++)
  {
    lightSamples[i] = sampler.getSamples().front();
    lightIndex[i] = gsl_rng_uniform_int(rng, light.size());
  }
  return integrate(path, scene, light, lightSamples, lightIndex);
}

Vec3f UniDiPathTracingIntegrator::integrate(const Path &path, const Intersectable &scene, const std::vector<const Light*> light, const Sample lightSamples[], long unsigned int lightIndex[]) const
{
  Vec3f color;

  std::list<Vec3f>::const_iterator alphaIt = path.alphaValues.begin();
  std::list<HitRecord>::const_iterator hitIt = path.hitRecords.begin();

  int i = 0;

  while(alphaIt != path.alphaValues.end() && hitIt != path.hitRecords.end())
  {
    QVector3D direction;

    Vec3f lightIntensity = light[lightIndex[i]]->getIntensity(hitIt->getIntersectingPoint(), direction, scene, lightSamples[i]);
    float inCos;
    inCos = QVector3D::dotProduct(-direction.normalized(), hitIt->getSurfaceNormal().normalized());
    Vec3f brdf;
    if(inCos > 0)
    {
//      assert(!isnan(lightIntensity.x()) && !isnan(lightIntensity.y()) && !isnan(lightIntensity.z()));
      brdf = hitIt->getMaterial().shade(*hitIt, direction);
//      assert(!isnan(brdf.x()) && !isnan(brdf.y()) && !isnan(brdf.z()));
//      assert(!isnan(alphaIt->x()) && !isnan(alphaIt->y()) && !isnan(alphaIt->z()));
//      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      color += alphaIt->mul(brdf).mul(lightIntensity) * inCos;
    }

    alphaIt++;
    hitIt++;
    i++;
  }

  return color;
}
