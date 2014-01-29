/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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

  if(hitIt != path.hitRecords.end() && hitIt->getMaterial().emission(*hitIt) != cv::Vec3f())
  {
      return hitIt->getMaterial().emission(*hitIt);
  }

  int i = 0;

  while(alphaIt != path.alphaValues.end() && hitIt != path.hitRecords.end())
  {
    QVector3D direction;
    Vec3f lightIntensity;
    auto nextHitIt = hitIt;
    nextHitIt++;
    HitRecord nextHit = *nextHitIt;
    Vec3f brdf;
    if(nextHitIt != path.hitRecords.end() && nextHit.getMaterial().emitsLight())
    {
        lightIntensity = nextHit.getMaterial().emission(nextHit);
        QVector4D lightPos = nextHit.getIntersectingPoint();
        if(lightPos.w() == 0)
        {
            direction = -lightPos.toVector3D();
        }
        else
        {
            direction = hitIt->getIntersectingPoint().toVector3DAffine() - lightPos.toVector3DAffine();
        }
    }
    else
    {
        lightIntensity = light[lightIndex[i]]->getIntensity(*hitIt, direction, scene, lightSamples[i]);
    }
    brdf = hitIt->getMaterial().brdf(*hitIt, direction);
    float inCos = QVector3D::dotProduct(-direction.normalized(), hitIt->getSurfaceNormal().normalized());
    float outCos = QVector3D::dotProduct(-hitIt->getRay().getDirection().normalized(), hitIt->getSurfaceNormal().normalized());
    if(signum(inCos) == signum(outCos))
    {
      assert(!isnan(lightIntensity[0]) && !isnan(lightIntensity[1]) && !isnan(lightIntensity[2]));
      assert(!isnan(brdf[0]) && !isnan(brdf[1]) && !isnan(brdf[2]));
      assert(!isnan((*alphaIt)[0]) && !isnan((*alphaIt)[1]) && !isnan((*alphaIt)[2]));
      assert(brdf[0] >= 0 && brdf[1] >= 0 && brdf[2] >= 0);
      assert(lightIntensity[0] >= 0 && lightIntensity[1] >= 0 && lightIntensity[2] >= 0);
      assert((*alphaIt)[0] >= 0 && (*alphaIt)[1] >= 0 && (*alphaIt)[2] >= 0);
      color += alphaIt->mul(brdf).mul(lightIntensity) * fabs(inCos);
    }

    alphaIt++;
    hitIt++;
    i++;
  }

  return color;
}
