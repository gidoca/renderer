#include "unidipathtracingintegrator.h"

#include "jitteredsampler.h"
#include "path.h"
#include "light.h"
#include "hitrecord.h"

#include <cmath>
#include <cassert>

Spectrum UniDiPathTracingIntegrator::integrate(const Ray& ray, const Intersectable& scene, const Light& light, int) const
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

