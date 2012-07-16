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

#ifdef NDEBUG
#include <omp.h>
#endif

#include <QTime>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace boost::program_options;
using namespace cv;

MetropolisFltRenderer::MetropolisFltRenderer() :
#ifdef NDEBUG
    numThreads(omp_get_max_threads())
#else
    numThreads(1)
#endif
{
}

Point2i getPos(const Sample& cameraSample, Size size)
{
    int x = (int)(cameraSample.getSample().x() * size.width);
    int y = (int)(cameraSample.getSample().y() * size.height);
    assert(0 <= x && x < film.size().width);
    assert(0 <= y && y < film.size().height);
    return Point2i(x, y);
}

void addSample(const Sample &cameraSample, float weight, Mat &film, Mat &mean, Mat &m2, Mat &sumweight, Vec3f value)
{
    Point2i pos = getPos(cameraSample, film.size());
    Vec3f depositValue = weight * value;

#pragma omp critical
    {
        float oldSumWeight = sumweight.at<float>(pos);
        float newSumWeight = weight + oldSumWeight;
        Vec3f delta = value - mean.at<Vec3f>(pos);
        Vec3f R = delta * (weight / newSumWeight);
        mean.at<Vec3f>(pos) += R;
        m2.at<Vec3f>(pos) += oldSumWeight * delta.mul(R);
        sumweight.at<float>(pos) = newSumWeight;
        film.at<Vec3f>(pos) += depositValue;
    }
}

void MetropolisFltRenderer::renderStep(Size size, const Scene& scene, vector<Mat>& films, vector<Mat>& biased_var, vector<Mat>& biased_mean, vector<Mat>& biased_m2)
{
    renderStep(size, scene, Mat::ones(size, CV_32F), films, biased_var, biased_mean, biased_m2);
}

void MetropolisFltRenderer::renderStep(Size size, const Scene& scene, Mat importanceMap, vector<Mat>& films, vector<Mat>& biased_var, vector<Mat>& biased_mean, vector<Mat>& biased_m2)
{
  const int numInitialSamples = vm["metflt-bootstrap"].as<int>();
  const float largeStepProb = vm["metflt-large-step-prob"].as<float>();
  const int numPixelSamples = vm["metflt-mutations"].as<int>() / vm["metflt-num-passes"].as<int>();

  gsl_rng *globalrng = gsl_rng_alloc(gsl_rng_taus);
  const int seed = getSeed(vm);
  gsl_rng_set(globalrng, seed);

  MetropolisSample initialSample(scene.light.size());
  initialSample.largeStep(globalrng);
  UniDiPathTracingIntegrator integrator;

  const int num_films = 2;

  float sumI = 0;
  vector<float> bootstrapI;
  vector<MetropolisSample> bootstrapSamples;
  for(int i = 0; i < numInitialSamples; i++)
  {
    initialSample.largeStep(globalrng);
    Path initialPath = initialSample.cameraPathFromSample(*scene.object, scene.camera);
    Vec3f l = integrator.integrate(initialPath, *scene.object, scene.light, initialSample.lightSample1, initialSample.lightIndex);
    sumI += norm(l) * importanceMap.at<float>(getPos(initialSample.cameraSample, size));
    bootstrapI.push_back(norm(l) * importanceMap.at<float>(getPos(initialSample.cameraSample, size)));
    bootstrapSamples.push_back(initialSample);
  }
  const float b = sumI / numInitialSamples;

  const float contribOffset = gsl_rng_uniform(globalrng) * sumI;
  sumI = 0;
  for(int i = 0; i < numInitialSamples; i++)
  {
    initialSample.largeStep(globalrng);
    initialSample = bootstrapSamples[i];
    sumI += bootstrapI[i];
    if(sumI > contribOffset) break;
  }

  Mat sumweight[num_films];
  for(int n = 0; n < num_films; n++)
  {
    sumweight[n] = Mat::zeros(size, CV_32FC1);
  }

  const int numSamples = numPixelSamples * size.area() / (numThreads);

#pragma omp parallel for
  for(int t = 0; t < numThreads; t++)
  {
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(rng, numThreads * seed + t);
    for(int n = 0; n < num_films; n++)
    {
      MetropolisSample currentSample = initialSample;
      Vec3f currentValue = integrator.integrate(initialSample.cameraPathFromSample(*scene.object, scene.camera), *scene.object, scene.light, initialSample.lightSample1, initialSample.lightIndex);

      for(int i = 0; i < numSamples; i++)
      {
          MetropolisSample newSample = currentSample.mutated(rng, largeStepProb);
          Vec3f newValue = integrator.integrate(newSample.cameraPathFromSample(*scene.object, scene.camera), *scene.object, scene.light, newSample.lightSample1, newSample.lightIndex);
          float accept = min(1., norm(newValue) * importanceMap.at<float>(getPos(newSample.cameraSample, size)) / (norm(currentValue) * importanceMap.at<float>(getPos(currentSample.cameraSample, size))));
          assert(!isnan(accept));

          if(norm(currentValue) > 0 && accept < 1)
          {
              addSample(currentSample.cameraSample, 1 - accept, films[n], biased_mean[n], biased_m2[n], sumweight[n], currentValue * (1 / (norm(currentValue) * importanceMap.at<float>(getPos(currentSample.cameraSample, size))) * b / numPixelSamples));
          }
          if(norm(newValue) > 0 && accept > 0)
          {
              addSample(newSample.cameraSample, accept, films[n], biased_mean[n], biased_m2[n], sumweight[n], newValue * (1 / (norm(newValue) * importanceMap.at<float>(getPos(newSample.cameraSample, size))) * b / numPixelSamples));
          }
          if(gsl_rng_uniform(rng) < accept)
          {
              currentSample = newSample;
              currentValue = newValue;
          }
      }
    }
    gsl_rng_free(rng);
  }

#pragma omp parallel for
  for(int n = 0; n < num_films; n++)
  {
    Mat sumweight3;
    merge(std::vector<Mat>(3, sumweight[n] + 1e-8f), sumweight3);
    biased_var[n] = biased_m2[n] / sumweight3;
  }
}

void MetropolisFltRenderer::render(const Scene & scene, Mat & film, const boost::program_options::variables_map vm)
{
  this->vm = vm;
  numPasses = vm["metflt-num-passes"].as<int>();

  QTime time;
  time.start();

  if(vm.count("verbose"))
  {
    cout << "Starting initial rendering" << endl;
  }

  const int num_films = 2;
  vector<Mat> biased_mean(num_films), biased_m2(num_films);
  vector<Mat> films(num_films), biased_var(num_films);

  for(int n = 0; n < num_films; n++)
  {
    films[n] = Mat::zeros(film.size(), CV_32FC3);
    biased_mean[n] = Mat::zeros(film.size(), CV_32FC3);
    biased_m2[n] = Mat::zeros(film.size(), CV_32FC3);
  }

  renderStep(film.size(), scene, films, biased_var, biased_mean, biased_m2);

  if(vm.count("verbose"))
  {
    cout << "Initial rendering complete, " << time.elapsed() / 1000 << "s elapsed, starting filtering" << endl;
  }

  Mat noisy_variance;
  var(film, noisy_variance, films);

  assert(checkRange(noisy_variance));

  Mat meanvar, varvar;
  var(meanvar, varvar, biased_var);
  assert(checkRange(meanvar));
  assert(checkRange(varvar));

  SymmetricFilter f;

  Mat filteredVar;
  filteredVar = f.filter(noisy_variance, meanvar, varvar);

  vector<Mat> filteredFilms(2);
  Mat varOfFiltered;
  filteredFilms[0] = (f.filter(films[0], films[1], filteredVar));
  filteredFilms[1] = (f.filter(films[1], films[0], filteredVar));
  if(!vm.count("metflt-omit-filter"))
  {
      film = filteredFilms[0] * .5f + filteredFilms[1] * .5f;
  }

  for(int i = 0; i < vm["metflt-num-passes"].as<int>() - 1; i++)
  {
    var(film, varOfFiltered, filteredFilms);
    Mat importanceMap = channelMean(varOfFiltered) / (channelMean(film) + 0.0001) + 0.0001;
    renderStep(film.size(), scene, importanceMap, films, biased_var, biased_mean, biased_m2);
    Mat newOut;
    var(newOut, noisy_variance, films);
    var(meanvar, varvar, biased_var);
    filteredVar = f.filter(noisy_variance, meanvar, varvar);
    filteredFilms[0] = f.filter(films[0], films[1], filteredVar);
    filteredFilms[1] = f.filter(films[1], films[0], filteredVar);

    if(vm.count("metflt-omit-filter"))
    {
        film = newOut;
    }
    else
    {
        film = filteredFilms[0] * .5f + filteredFilms[1] * .5f;
    }
  }
}

options_description MetropolisFltRenderer::options()
{
  options_description opts("Metropolis-Filter renderer options");
  opts.add_options()
      ("metflt-large-step-prob", value<float>()->default_value(0.1f, "0.1"), "the probability for a mutation to be a large step mutation")
      ("metflt-bootstrap", value<int>()->default_value(1000), "number of bootstrapping samples")
      ("metflt-mutations", value<int>()->default_value(16), "average number of path mutations per pixel")
      ("metflt-fixed-seed", "use a fixed seed for the RNG to make the resulting image deterministic")
      ("metflt-omit-filter", "don't filter the image")
      ("metflt-num-passes", value<int>()->default_value(3), "the number of filtering passes");
  return opts;
}

const std::string MetropolisFltRenderer::name = "metropolisflt";
