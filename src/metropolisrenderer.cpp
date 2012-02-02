#include "metropolisrenderer.h"

#include "jitteredsampler.h"
#include "camera.h"
#include "path.h"
#include "unidipathtracingintegrator.h"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

void MetropolisRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< Light* > lights, Film & film)
{
  gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set(rng, 2);

  MetropolisSample sample(lights.size());
  sample.largeStep(rng);
  Path path = cameraPathFromSample(sample, scene, camera);
  UniDiPathTracingIntegrator integrator;
  Spectrum value = integrator.integrate(path, scene, *lights[sample.lightIndex[0]], sample.lightSample1);

  const int numSamples = 10000000;
  const float largeStepProb = 0.1f;
  for(int i = 0; i < numSamples; i++)
  {
    MetropolisSample newSample = sample.mutated(rng, largeStepProb);
    Spectrum newValue = integrator.integrate(path, scene, *lights[sample.lightIndex[0]], sample.lightSample1);
    float accept = std::min(1., newValue.length() / value.length());
    int x = (int)(sample.cameraSample.getSample().x() * film.width());
    int y = (int)(sample.cameraSample.getSample().y() * film.height());
    film[y][x] += (1 - accept) * value / (value.length() * numSamples) * film.getSize().width() * film.getSize().height();
    x = (int)(newSample.cameraSample.getSample().x() * film.width());
    y = (int)(newSample.cameraSample.getSample().y() * film.height());
    film[y][x] += accept * newValue / (newValue.length() * numSamples) * film.getSize().width() * film.getSize().height();
    if(gsl_rng_uniform(rng) < accept)
    {
      sample = newSample;
      value = newValue;
      path = cameraPathFromSample(sample, scene, camera);
    }
  }
}

Path MetropolisRenderer::cameraPathFromSample(MetropolisSample sample, const Intersectable & scene, const Camera& camera)
{
  QPointF pixel = sample.cameraSample.getSample();
  pixel.rx() *= film.getSize().width();
  pixel.ry() *= film.getSize().height();
  Path result = createPath(camera.getRay(pixel), scene, sample.cameraPathSamples);
  return result;
}

void MetropolisSample::largeStep(gsl_rng *rng)
{
  JitteredSampler sampler(1, 1, rng);
  cameraSample = sampler.getSamples().front();

  for(int i = 0; i < MAX_DEPTH; i++)
  {
    lightPathSamples[i] = sampler.getSamples().front();
    cameraPathSamples[i] = sampler.getSamples().front();
    lightSample1[i] = sampler.getSamples().front();
    lightSample2[i] = sampler.getSamples().front();
    lightIndex[i] = gsl_rng_uniform_int(rng, numLights);
  }

}

void mutate(qreal &s, gsl_rng *rng)
{
  JitteredSampler sampler(1, 1, rng);
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

void MetropolisSample::smallStep(gsl_rng *rng)
{
  mutate(cameraSample.getSample().rx(), rng);
  mutate(cameraSample.getSample().ry(), rng);
  for(int i = 0; i < MAX_DEPTH; i++)
  {
      mutate(lightPathSamples[i].getSample().rx(), rng);
      mutate(lightPathSamples[i].getSample().ry(), rng);
      mutate(cameraPathSamples[i].getSample().rx(), rng);
      mutate(cameraPathSamples[i].getSample().ry(), rng);
      mutate(lightSample1[i].getSample().rx(), rng);
      mutate(lightSample1[i].getSample().ry(), rng);
      mutate(lightSample2[i].getSample().rx(), rng);
      mutate(lightSample2[i].getSample().ry(), rng);
  }
}

MetropolisSample MetropolisSample::mutated(gsl_rng *rng, float largeStepProb)
{
  MetropolisSample result = *this;
  float s = gsl_rng_uniform(rng);
  if(s < largeStepProb)
  {
    result.largeStep(rng);
  }
  else
  {
    result.smallStep(rng);
  }
  return result;
}
