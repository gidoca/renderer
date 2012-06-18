#include "metropolisrenderer.h"

#include "jitteredsampler.h"
#include "camera.h"
#include "path.h"
#include "unidipathtracingintegrator.h"
#include "metropolissample.h"
#include "symmetricfilter.h"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <omp.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace boost::program_options;
using namespace cv;

void MetropolisRenderer::render(const Scene & scene, Mat & film, const boost::program_options::variables_map vm)
{
  const int num_films = 2;
  Mat films[num_films];
  Mat sum = Mat::zeros(film.size(), film.type()), sqr_sum = Mat::zeros(film.size(), film.type());
  gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
  int seed = getSeed(vm);
  gsl_rng_set(rng, seed);

  MetropolisSample sample(scene.light.size());
  sample.largeStep(rng);
  Path path = sample.cameraPathFromSample(*scene.object, scene.camera);
  UniDiPathTracingIntegrator integrator;
  Vec3f value = integrator.integrate(path, *scene.object, scene.light, sample.lightSample1, sample.lightIndex);

  const int numInitialSamples = vm["met-bootstrap"].as<int>();
  const float largeStepProb = vm["met-large-step-prob"].as<float>();

  float sumI = 0;
  vector<float> bootstrapI;
  vector<MetropolisSample> bootstrapSamples;
  for(int i = 0; i < numInitialSamples; i++)
  {
    sample.largeStep(rng);
    path = sample.cameraPathFromSample(*scene.object, scene.camera);
    Vec3f l = integrator.integrate(path, *scene.object, scene.light, sample.lightSample1, sample.lightIndex);
    sumI += norm(l);
    bootstrapI.push_back(norm(l));
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
  const int numSamples = numPixelSamples * film.size().width * film.size().height / numThreads;

  for(int n = 0; n < num_films; n++)
  {
    films[n].create(film.size(), film.type());
#pragma omp parallel for
    for(int t = 0; t < numThreads; t++)
    {
        MetropolisSample currentSample = sample;
        Vec3f currentValue = value;
        Path currentPath = path;

        for(int i = 0; i < numSamples; i++)
        {
            MetropolisSample newSample = currentSample.mutated(rng, largeStepProb);
            currentPath = newSample.cameraPathFromSample(*scene.object, scene.camera);
            Vec3f newValue = integrator.integrate(currentPath, *scene.object, scene.light, newSample.lightSample1, newSample.lightIndex);
            float accept = min(1., norm(newValue) / norm(currentValue));
            assert(!isnan(accept));

            if(norm(currentValue) > 0)
            {
                int x = (int)(currentSample.cameraSample.getSample().x() * film.size().width);
                int y = (int)(currentSample.cameraSample.getSample().y() * film.size().height);
                assert(0 <= x && x < film.size().width);
                assert(0 <= y && y < film.size().height);
#pragma omp critical
                films[n].at<Vec3f>(y, x) += (1 - accept) * currentValue * (1 / norm(currentValue) * b / numPixelSamples);
            }
            if(norm(newValue) > 0)
            {
                int x = (int)(newSample.cameraSample.getSample().x() * film.size().width);
                int y = (int)(newSample.cameraSample.getSample().y() * film.size().height);
                assert(0 <= x && x < film.size().width);
                assert(0 <= y && y < film.size().height);
#pragma omp critical
                films[n].at<Vec3f>(y, x) += accept * newValue * (1 / norm(newValue) * b / numPixelSamples);
            }
            if(gsl_rng_uniform(rng) < accept)
            {
                currentSample = newSample;
                currentValue = newValue;
            }
        }
    }
  }

  for(int i = 0; i < num_films; i++)
  {
    sum += films[i];
    sqr_sum += films[i].mul(films[i]);
  }
  Mat mean = sum * (1. / num_films);
  Mat variance = max((sqr_sum - sum.mul(mean)) / (num_films - 1), 0);

  SymmetricFilter f;

  f.filter(mean, mean, variance).copyTo(film);
//  mean.copyTo(film);
  Mat cmean;
  cvtColor(mean, cmean, CV_RGB2BGR);
  imwrite("/home/gidoca/mean.exr", cmean);
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
