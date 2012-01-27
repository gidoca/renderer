#include "metropolisrenderer.h"

#include "jitteredsampler.h"
#include "camera.h"

#include <cmath>
#include <cassert>

void MetropolisRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< QSharedPointer< Light > > lights, Film & film)
{
}

Path MetropolisRenderer::cameraPathFromSample(MetropolisSample sample, const Intersectable & scene, const Camera& camera)
{
  QPointF pixel = sample.cameraSample.getSample();
  pixel.rx() *= film.getSize().width();
  pixel.ry() *= film.getSize().height();
  return createPath(camera.getRay(pixel), scene, sample.cameraPathSamples);
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
    lightIndex[i] = qrand();
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
