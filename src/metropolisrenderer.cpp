#include "metropolisrenderer.h"

#include "jitteredsampler.h"
#include "camera.h"
#include "path.h"

#include <cmath>
#include <cassert>

void MetropolisRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< Light* > lights, Film & film)
{
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
