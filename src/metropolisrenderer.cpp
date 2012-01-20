#include "metropolisrenderer.h"

#include "jitteredsampler.h"

#include <cmath>
#include <cassert>

Film MetropolisRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< QSharedPointer< Light > > lights)
{
  return film;
}

void MetropolisSample::largeStep()
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

void MetropolisSample::smallStep()
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