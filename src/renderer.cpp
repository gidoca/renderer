#include "renderer.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "jitteredsampler.h"

#include <cmath>
#include <cassert>

Path Renderer::createPath(const Ray &primaryRay, const Intersectable &scene, Spectrum initialAlpha)
{
    PathSample sample;
    sample.largeStep();
    return createPath(primaryRay, scene, sample.cameraPathSamples, initialAlpha);
}

Path Renderer::createPath(const Ray& primaryRay, const Intersectable &scene, Sample pathSamples[], Spectrum alpha)
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
  for(int i = 0; i < MAX_DEPTH; i++)
  {
    if(!hit.intersects()) return result;

#ifdef ROUSSIAN_ROULETTE
    const float terminationProb = 0.5;
    if(i >= 2 && qrand() < RAND_MAX * terminationProb) return result;
    float russianRoulettePdf = (i < 2 ? 1 : 1 - terminationProb);
#else
    const float russianRoulettePdf = 1;
#endif

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

void PathSample::largeStep()
{
  JitteredSampler sampler(1, 1);
  cameraSample = sampler.getSamples().front();

  for(int i = 0; i < MAX_DEPTH; i++)
  {
    lightPathSamples[i] = sampler.getSamples().front();
    cameraPathSamples[i] = sampler.getSamples().front();
    lightSample1[i] = sampler.getSamples().front();
    lightSample2[i] = sampler.getSamples().front();
  }
}

void mutate(qreal &s)
{
  JitteredSampler sampler(1, 1);
  Sample sample = sampler.getSamples().front();
  static const float a = 1. / 1024., b = 1. / 64.;
  static const float logRatio = -log(b / a);
  float delta = b * exp(logRatio * sample.getSample().x());
  if(sample.getSample().y() < 0.5)
  {
    delta *= -1;
  }

  s = fmod(s + delta, 1);
  assert(0 <= s && s <= 1);
}

void PathSample::smallStep()
{
  mutate(cameraSample.getSample().rx());
  mutate(cameraSample.getSample().ry());
  for(int i = 0; i < MAX_DEPTH; i++)
  {
      mutate(lightPathSamples[i].getSample().rx());
      mutate(lightPathSamples[i].getSample().ry());
      mutate(cameraPathSamples[i].getSample().rx());
      mutate(cameraPathSamples[i].getSample().ry());
      mutate(lightSample1[i].getSample().rx());
      mutate(lightSample1[i].getSample().ry());
      mutate(lightSample2[i].getSample().rx());
      mutate(lightSample2[i].getSample().ry());
  }
}
