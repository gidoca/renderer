#include "metropolisfltrenderer.h"

#include "jitteredsampler.h"
#include "camera.h"
#include "path.h"
#include "unidipathtracingintegrator.h"
#include "metropolissample.h"
#include "symmetricfilter.h"
#include "mathhelper.h"

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

void addSample(const Sample &cameraSample, float weight, Mat &film, Mat &mean, Mat &m2, Mat &sumweight, Vec3f value)
{
    int x = (int)(cameraSample.getSample().x() * film.size().width);
    int y = (int)(cameraSample.getSample().y() * film.size().height);
    assert(0 <= x && x < film.size().width);
    assert(0 <= y && y < film.size().height);

    Vec3f depositValue = weight * value;

#pragma omp critical
    film.at<Vec3f>(y, x) += depositValue;

    float oldSumWeight = sumweight.at<float>(y, x);
    float newSumWeight = weight + oldSumWeight;
    Vec3f delta = value - mean.at<Vec3f>(y, x);
    Vec3f R = delta * (weight / newSumWeight);
    mean.at<Vec3f>(y, x) += R;
    m2.at<Vec3f>(y, x) += oldSumWeight * delta.mul(R);
    sumweight.at<float>(y, x) = newSumWeight;
}

void MetropolisFltRenderer::render(const Scene & scene, Mat & film, const boost::program_options::variables_map vm)
{
  const int num_films = 2;
  Mat biased_mean[num_films], biased_m2[num_films];
  vector<Mat> films(num_films), biased_var(num_films);
  Mat sumweight[num_films];

  gsl_rng *globalrng = gsl_rng_alloc(gsl_rng_taus);
  const int seed = getSeed(vm);
  gsl_rng_set(globalrng, seed);

  MetropolisSample sample(scene.light.size());
  sample.largeStep(globalrng);
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
    sample.largeStep(globalrng);
    path = sample.cameraPathFromSample(*scene.object, scene.camera);
    Vec3f l = integrator.integrate(path, *scene.object, scene.light, sample.lightSample1, sample.lightIndex);
    sumI += norm(l);
    bootstrapI.push_back(norm(l));
    bootstrapSamples.push_back(sample);
  }
  const float b = sumI / numInitialSamples;

  const float contribOffset = gsl_rng_uniform(globalrng) * sumI;
  sumI = 0;
  for(int i = 0; i < numInitialSamples; i++)
  {
    sample.largeStep(globalrng);
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

#pragma omp parallel for
  for(int n = 0; n < num_films; n++)
  {
    films[n] = Mat::zeros(film.size(), film.type());
    biased_mean[n] = Mat::zeros(film.size(), film.type());
    biased_m2[n] = Mat::zeros(film.size(), film.type());
    sumweight[n] = Mat::zeros(film.size(), CV_32FC1);
  }


#pragma omp parallel for
  for(int t = 0; t < numThreads; t++)
  {
      gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
      gsl_rng_set(rng, numThreads * seed + t);
      for(int n = 0; n < num_films; n++)
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
                addSample(currentSample.cameraSample, 1 - accept, films[n], biased_mean[n], biased_m2[n], sumweight[n], currentValue * (1 / norm(currentValue) * b / numPixelSamples));
            }
            if(norm(newValue) > 0)
            {
                addSample(currentSample.cameraSample, accept, films[n], biased_mean[n], biased_m2[n], sumweight[n], newValue * (1 / norm(newValue) * b / numPixelSamples));
            }
            if(gsl_rng_uniform(rng) < accept)
            {
                currentSample = newSample;
                currentValue = newValue;
            }
        }
      }
  }

#pragma omp parallel for
  for(int n = 0; n < num_films; n++)
  {
    Mat sumweight3;
    merge(std::vector<Mat>(3, sumweight[n]), sumweight3);
    biased_var[n] = biased_m2[n] / sumweight3;
  }


  Mat noisy_variance;
  var(film, noisy_variance, films);
  Mat noisy_mean = film;

  Mat meanvar, varvar;
  var(meanvar, varvar, biased_var);

  SymmetricFilter f;

  Mat filteredVar;
  filteredVar = f.filter(noisy_variance, meanvar, varvar);

  imwrite("/tmp/mean.exr", noisy_mean);

  Mat film1, film2;
  film1 = f.filter(films[0], films[1], filteredVar);
  film2 = f.filter(films[1], films[0], filteredVar);
  Mat m = ((film1 + film2) / 2.);
  m.copyTo(film);
  imwrite("/tmp/film0.exr", films[0]);
  imwrite("/tmp/film1.exr", films[1]);
}

options_description MetropolisFltRenderer::options()
{
  options_description opts("Metropolis-Filter renderer options");
  opts.add_options()
      ("metflt-large-step-prob", value<float>()->default_value(0.1f, "0.1"), "the probability for a mutation to be a large step mutation")
      ("metflt-bootstrap", value<int>()->default_value(1000), "number of bootstrapping samples")
      ("metflt-mutations", value<int>()->default_value(16), "average number of path mutations per pixel")
      ("metflt-fixed-seed", "use a fixed seed for the RNG to make the resulting image deterministic");
  return opts;
}

const std::string MetropolisFltRenderer::name = "metropolisflt";
