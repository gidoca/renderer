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

#include <omp.h>

using namespace std;
using namespace boost::program_options;

void MetropolisRenderer::render(const Scene & scene, Film & film, const boost::program_options::variables_map vm)
{
  const int num_films = 100;
  Film films[num_films];
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

  const int numPixelSamples = vm["met-mutations"].as<int>();
#ifdef NDEBUG
  const int numThreads = omp_get_max_threads();
#else
  const int numThreads = 1;
#endif
  const int numSamples = numPixelSamples * film.getSize().width() * film.getSize().height() / numThreads;

  for(int n = 0; n < num_films; n++)
  {
    films[n] = Film(film.getSize());
#pragma omp parallel for
  for(int t = 0; t < numThreads; t++)
  {
      MetropolisSample currentSample = sample;
      Spectrum currentValue = value;
      Path currentPath = path;

      for(int i = 0; i < numSamples; i++)
      {
          MetropolisSample newSample = currentSample.mutated(rng, largeStepProb);
          currentPath = newSample.cameraPathFromSample(*scene.object, scene.camera);
          Spectrum newValue = integrator.integrate(currentPath, *scene.object, scene.light, newSample.lightSample1, newSample.lightIndex);
          assert(!isnan(newValue.x()) && !isnan(newValue.y()) && !isnan(newValue.z()));
          float accept = min(1., newValue.length() / currentValue.length());
          assert(!isnan(accept));

          if(currentValue.length() > 0)
          {
              int x = (int)(currentSample.cameraSample.getSample().x() * film.width());
              int y = (int)(currentSample.cameraSample.getSample().y() * film.height());
              assert(0 <= x && x < film.width());
              assert(0 <= y && y < film.height());
#pragma omp critical
              films[n][y][x] += (1 - accept) * currentValue / currentValue.length() * b / numPixelSamples;
          }
          if(newValue.length() > 0)
          {
              int x = (int)(newSample.cameraSample.getSample().x() * film.width());
              int y = (int)(newSample.cameraSample.getSample().y() * film.height());
              assert(0 <= x && x < film.width());
              assert(0 <= y && y < film.height());
#pragma omp critical
              films[n][y][x] += accept * newValue / newValue.length() * b / numPixelSamples;
          }
          if(gsl_rng_uniform(rng) < accept)
          {
              currentSample = newSample;
              currentValue = newValue;
          }
      }
  }
  }

  for(int i = 0; i < film.height(); i++)
  {
    for(int j = 0; j < film.width(); j++)
    {
//      Spectrum diff = films[0][i][j] - films[1][i][j];
//      Spectrum variance = diff * diff / 4;
//      film[i][j] = variance;
//      film[i][j] = films[0][i][j];
        Spectrum estimated_mean, estimated_squared_mean;
        for(int k = 0; k < num_films; k++)
        {
            estimated_mean += films[k][i][j] / (num_films - 1);
            estimated_squared_mean += films[k][i][j] * films[k][i][j] / (num_films - 1);
        }
        film[i][j] = estimated_squared_mean - estimated_mean * estimated_mean;
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
