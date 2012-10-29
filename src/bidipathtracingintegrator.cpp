#include "bidipathtracingintegrator.h"

#include "light.h"
#include "jitteredsampler.h"
#include "path.h"
#include "ray.h"
#include "hitrecord.h"
#include "intersectable.h"
#include "renderer.h"

#include <list>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace cv;

Vec3f BiDiPathTracingIntegrator::integrate(const Ray &ray, const Intersectable &scene, const std::vector<const Light*> light, int, gsl_rng *rng) const
{
  Vec3f color;
  JitteredSampler sampler(1, 1, rng);
  float directionPdf;
  int lightIndex = gsl_rng_uniform_int(rng, light.size());
  Ray primaryLightRay = light[lightIndex]->getRandomRay(sampler.getSamples().front(), sampler.getSamples().front(), directionPdf);
  HitRecord initialLightHit = scene.intersect(primaryLightRay);
  QVector3D initialLightDirection;
  Vec3f lightIntensity = light[lightIndex]->getIntensity(initialLightHit.getIntersectingPoint(), initialLightDirection, scene, sampler.getSamples().front());

  Path lightPath = Renderer::createPath(primaryLightRay, scene, rng, lightIntensity * (1 / directionPdf));
  Path eyePath = Renderer::createPath(ray, scene, rng);

  std::list<Vec3f>::iterator lightAlphaIt = lightPath.alphaValues.begin();
  std::list<HitRecord>::iterator lightHitIt = lightPath.hitRecords.begin();
  int lightInd = 1;

  while(lightAlphaIt != lightPath.alphaValues.end() && lightHitIt != lightPath.hitRecords.end())
  {
    std::list<Vec3f>::iterator eyeAlphaIt = eyePath.alphaValues.begin();
    std::list<HitRecord>::iterator eyeHitIt = eyePath.hitRecords.begin();
    int eyeInd = 1;

    while(eyeAlphaIt != eyePath.alphaValues.end() && eyeHitIt != eyePath.hitRecords.end())
    {
      QVector3D connDirection = lightHitIt->getIntersectingPoint() - eyeHitIt->getIntersectingPoint();
      float geometryTerm = QVector3D::dotProduct(connDirection.normalized(), eyeHitIt->getSurfaceNormal().normalized()) * QVector3D::dotProduct(-connDirection.normalized(), lightHitIt->getSurfaceNormal().normalized()) / connDirection.lengthSquared();
      if(geometryTerm > 0)
      {
        Vec3f eyeBrdf = eyeHitIt->getMaterial().shade(*eyeHitIt, connDirection);
        Vec3f lightBrdf = lightHitIt->getMaterial().shade(*lightHitIt, -connDirection);
        int pathLength = eyeInd + lightInd + 1;
        int nPaths = std::min<int>(pathLength - 1, eyePath.alphaValues.size()) + std::min<int>(pathLength - 2, lightPath.alphaValues.size()) + 2 - pathLength;
        assert(nPaths > 0);
        color += eyeAlphaIt->mul(*lightAlphaIt).mul(eyeBrdf).mul(lightBrdf) * (geometryTerm / nPaths);
      }

      eyeAlphaIt++;
      eyeHitIt++;
      eyeInd++;
    }
    lightAlphaIt++;
    lightHitIt++;
    lightInd++;
  }

  std::list<Vec3f>::iterator eyeAlphaIt = eyePath.alphaValues.begin();
  std::list<HitRecord>::iterator eyeHitIt = eyePath.hitRecords.begin();
  int eyeInd = 2;

  while(eyeAlphaIt != eyePath.alphaValues.end() && eyeHitIt != eyePath.hitRecords.end())
  {
    QVector3D lightDirection;
    lightIntensity = light[lightIndex]->getIntensity(eyeHitIt->getIntersectingPoint(), lightDirection, scene, sampler.getSamples().front());

    float inCos = QVector3D::dotProduct(-lightDirection.normalized(), eyeHitIt->getSurfaceNormal().normalized());
    Vec3f brdf;
    if(inCos > 0)
    {
//      assert(!isnan(lightIntensity.x()) && !isnan(lightIntensity.y()) && !isnan(lightIntensity.z()));
      brdf = eyeHitIt->getMaterial().shade(*eyeHitIt, lightDirection);
//      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      int nPaths = std::min<int>(eyeInd - 1, eyePath.alphaValues.size()) + 2 - eyeInd;
      assert(nPaths > 0);
//      int nPaths = eyeInd;
      color += eyeAlphaIt->mul(brdf).mul(lightIntensity) * (inCos / nPaths);
    }

    eyeAlphaIt++;
    eyeHitIt++;
    eyeInd++;
  }

  return color;
}
