#include "metropolisrenderer.h"

#include "jitteredsampler.h"
#include "camera.h"
#include "path.h"
#include "unidipathtracingintegrator.h"
#include "metropolissample.h"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <QTime>

using namespace std;
using namespace boost::program_options;

void MetropolisRenderer::render(const Scene & scene, Film & film, const boost::program_options::variables_map vm)
{
  gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
  int seed = getSeed(vm);
  gsl_rng_set(rng, seed);

  MetropolisSample sample(scene.light.size());
  sample.largeStep(rng);
  Path path = sample.cameraPathFromSample(*scene.object, scene.camera);
  UniDiPathTracingIntegrator integrator;
  Spectrum value = integrator.integrate(path, *scene.object, scene.light, sample.lightSample1, sample.lightIndex);

  const int numInitialSamples = vm["met-bootstrap"].as<int>();
  const float largeStepProb = vm["met-large-step-prob"].as<float>();

  float sumI = 0;
  vector<float> bootstrapI;
  vector<MetropolisSample> bootstrapSamples;
  for(int i = 0; i < numInitialSamples; i++)
  {
    sample.largeStep(rng);
    path = sample.cameraPathFromSample(*scene.object, scene.camera);
    Spectrum l = integrator.integrate(path, *scene.object, scene.light, sample.lightSample1, sample.lightIndex);
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
    sample = bootstrapSamples[i];
    sumI += bootstrapI[i];
    if(sumI > contribOffset) break;
  }
//  sample.largeStep(rng);

  int numPixelSamples = vm["met-mutations"].as<int>();
  const int numSamples = numPixelSamples * film.getSize().width() * film.getSize().height();

  for(int i = 0; i < numSamples; i++)
  {
    MetropolisSample newSample = sample.mutated(rng, largeStepProb);
    path = newSample.cameraPathFromSample(*scene.object, scene.camera);
    Spectrum newValue = integrator.integrate(path, *scene.object, scene.light, newSample.lightSample1, newSample.lightIndex);
    assert(!isnan(newValue.x()) && !isnan(newValue.y()) && !isnan(newValue.z()));
    float accept = min(1., newValue.length() / value.length());
    assert(!isnan(accept));
    
    if(value.length() > 0)
    {
      int x = (int)(sample.cameraSample.getSample().x() * film.width());
      int y = (int)(sample.cameraSample.getSample().y() * film.height());
      assert(0 <= x && x < film.width());
      assert(0 <= y && y < film.height());
      film[y][x] += (1 - accept) * value / value.length() * b / numPixelSamples;
    }
    if(newValue.length() > 0)
    {
      int x = (int)(newSample.cameraSample.getSample().x() * film.width());
      int y = (int)(newSample.cameraSample.getSample().y() * film.height());
      assert(0 <= x && x < film.width());
      assert(0 <= y && y < film.height());
      film[y][x] += accept * newValue / newValue.length() * b / numPixelSamples;
    }
    if(gsl_rng_uniform(rng) < accept)
    {
      sample = newSample;
      value = newValue;
    }
  }
}

options_description MetropolisRenderer::options()
{
  options_description opts("Metropolis renderer options");
  opts.add_options()
      ("met-large-step-prob", value<float>()->default_value(0.1f, "0.1"), "the probability for a mutation to be a large step mutation")
      ("met-bootstrap", value<int>()->default_value(1000), "number of bootstrapping samples")
      ("met-mutations", value<int>()->default_value(16), "average number of path mutations per pixel")
      ("met-fixed-seed", "use a fixed seed for the RNG to make the resulting image deterministic");
  return opts;
}

const std::string MetropolisRenderer::name = "metropolis";
