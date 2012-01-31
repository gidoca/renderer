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

Spectrum BiDiPathTracingIntegrator::integrate(const Ray &ray, const Intersectable &scene, const Light &light, int recursionDepth, gsl_rng *rng) const
{
  Spectrum color;
  JitteredSampler sampler(1, 1, rng);
  float directionPdf;
  Ray primaryLightRay = light.getRandomRay(sampler.getSamples().front(), sampler.getSamples().front(), directionPdf);
  HitRecord initialLightHit = scene.intersect(primaryLightRay);
  QVector3D initialLightDirection;
  Spectrum lightIntensity = light.getIntensity(initialLightHit.getIntersectingPoint(), initialLightDirection, scene, sampler.getSamples().front());

  Path lightPath = Renderer::createPath(primaryLightRay, scene, rng, lightIntensity / directionPdf);
  Path eyePath = Renderer::createPath(ray, scene, rng);

  std::list<Spectrum>::iterator lightAlphaIt = lightPath.alphaValues.begin();
  std::list<HitRecord>::iterator lightHitIt = lightPath.hitRecords.begin();
  int lightInd = 1;

  while(lightAlphaIt != lightPath.alphaValues.end() && lightHitIt != lightPath.hitRecords.end())
  {
    std::list<Spectrum>::iterator eyeAlphaIt = eyePath.alphaValues.begin();
    std::list<HitRecord>::iterator eyeHitIt = eyePath.hitRecords.begin();
    int eyeInd = 1;

    while(eyeAlphaIt != eyePath.alphaValues.end() && eyeHitIt != eyePath.hitRecords.end())
    {
      QVector3D connDirection = lightHitIt->getIntersectingPoint() - eyeHitIt->getIntersectingPoint();
      float geometryTerm = QVector3D::dotProduct(connDirection.normalized(), eyeHitIt->getSurfaceNormal().normalized()) * QVector3D::dotProduct(-connDirection.normalized(), lightHitIt->getSurfaceNormal().normalized()) / connDirection.lengthSquared();
      if(geometryTerm > 0)
      {
        Spectrum eyeBrdf = eyeHitIt->getMaterial().shade(*eyeHitIt, connDirection);
        Spectrum lightBrdf = lightHitIt->getMaterial().shade(*lightHitIt, -connDirection);

        color += *eyeAlphaIt * *lightAlphaIt * eyeBrdf * lightBrdf * geometryTerm / (eyeInd + lightInd);
      }

      eyeAlphaIt++;
      eyeHitIt++;
      eyeInd++;
    }
    lightAlphaIt++;
    lightHitIt++;
    lightInd++;
  }

  std::list<Spectrum>::iterator eyeAlphaIt = eyePath.alphaValues.begin();
  std::list<HitRecord>::iterator eyeHitIt = eyePath.hitRecords.begin();
  int eyeInd = 1;

  while(eyeAlphaIt != eyePath.alphaValues.end() && eyeHitIt != eyePath.hitRecords.end())
  {
    QVector3D lightDirection;
    lightIntensity = light.getIntensity(eyeHitIt->getIntersectingPoint(), lightDirection, scene, sampler.getSamples().front());

    float inCos = QVector3D::dotProduct(-lightDirection.normalized(), eyeHitIt->getSurfaceNormal().normalized());
    Spectrum brdf;
    if(inCos > 0)
    {
      assert(!isnan(lightIntensity.x()) && !isnan(lightIntensity.y()) && !isnan(lightIntensity.z()));
      brdf = eyeHitIt->getMaterial().shade(*eyeHitIt, lightDirection);
      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      color += *eyeAlphaIt * brdf * lightIntensity * inCos / eyeInd;
    }

    eyeAlphaIt++;
    eyeHitIt++;
    eyeInd++;
  }

  return color;
}
