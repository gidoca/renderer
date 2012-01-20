#include "metropolisrenderer.h"

#include "jitteredsampler.h"

Film MetropolisRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< QSharedPointer< Light > > lights)
{
  return film;
}

void MetropolisSample::largeStep()
{
  JitteredSampler sampler(1, 1);
  lightSample1 = sampler.getSamples().front();
  lightSample2 = sampler.getSamples().front();
  cameraSample = sampler.getSamples().front();
  
  for(int i = 0; i < MAX_DEPTH; i++)
  {
    lightPathSamples[i] = sampler.getSamples().front();
    cameraPathSamples[i] = sampler.getSamples().front();
  }
}

void MetropolisSample::smallStep()
{
  
}