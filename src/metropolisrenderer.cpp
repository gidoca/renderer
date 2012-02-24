#include "metropolisrenderer.h"

#include "jitteredsampler.h"
#include "camera.h"
#include "path.h"
#include "unidipathtracingintegrator.h"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <QTime>

using namespace std;

void MetropolisRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< Light* > lights, Film & film)
{
  QTime time;
  time.start();

  gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set(rng, 2);

  MetropolisSample sample(lights.size());
  sample.largeStep(rng);
  Path path = cameraPathFromSample(sample, scene, camera);
  UniDiPathTracingIntegrator integrator;
  Spectrum value = integrator.integrate(path, scene, lights, sample.lightSample1, sample.lightIndex);

  const int numInitialSamples = 1000;
  const float largeStepProb = 0.1f;

  float sumI = 0;
  vector<float> bootstrapI;
  vector<MetropolisSample> bootstrapSamples;
  for(int i = 0; i < numInitialSamples; i++)
  {
    sample.largeStep(rng);
    path = cameraPathFromSample(sample, scene, camera);
    Spectrum l = integrator.integrate(path, scene, lights, sample.lightSample1, sample.lightIndex);
    sumI += l.length();
    bootstrapI.push_back(l.length());
    bootstrapSamples.push_back(sample);
  }
  const float b = sumI / numInitialSamples;

  const float contribOffset = gsl_rng_uniform(rng) * sumI;
  sumI = 0;
  for(int i = 0; i < numInitialSamples; i++)
  {
    sample.largeStep(rng);
//    sample = bootstrapSamples[i];
    sumI += bootstrapI[i];
    if(sumI > contribOffset) break;
  }
//  sample.largeStep(rng);

  const int numPixelSamples = 16;
  const int numSamples = numPixelSamples * film.getSize().width() * film.getSize().height();

  for(int i = 0; i < numSamples; i++)
  {
    MetropolisSample newSample = sample.mutated(rng, largeStepProb);
    path = cameraPathFromSample(newSample, scene, camera);
    Spectrum newValue = integrator.integrate(path, scene, lights, newSample.lightSample1, newSample.lightIndex);
    assert(!isnan(newValue.x()) && !isnan(newValue.y()) && !isnan(newValue.z()));
    float accept = min(1., newValue.length() / value.length());
    assert(!isnan(accept));
    
    if(value.length() > 0)
    {
      int x = (int)(sample.cameraSample.getSample().x() * film.width());
      int y = (int)(sample.cameraSample.getSample().y() * film.height());
      film[y][x] += (1 - accept) * value / value.length() * b / numPixelSamples;
    }
    if(newValue.length() > 0)
    {
      int x = (int)(newSample.cameraSample.getSample().x() * film.width());
      int y = (int)(newSample.cameraSample.getSample().y() * film.height());
      film[y][x] += accept * newValue / newValue.length() * b / numPixelSamples;
    }
    if(gsl_rng_uniform(rng) < accept)
    {
      sample = newSample;
      value = newValue;
    }
  }

  std::cout << "Rendering complete, " << time.elapsed() / 1000 << "s elapsed\n";
  std::cout.flush();
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

  s = s + delta;
  if(s < 0) s += 1;
  if(s > 1) s -= 1;
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
