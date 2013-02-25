/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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

#ifdef USE_OPENMP
#include <omp.h>
#endif

#include <QTime>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace boost::program_options;
using namespace cv;

MetropolisFltRenderer::MetropolisFltRenderer() :
#ifdef USE_OPENMP
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
    assert(0 <= x && x < size.width);
    assert(0 <= y && y < size.height);
    return Point2i(x, y);
}

void MetropolisFltRenderer::addSample(const Sample &cameraSample, float weight, Mat &film, Mat &mean, Mat &m2, Mat &sumweight, Vec3f value)
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

void MetropolisFltRenderer::renderStep(Size size, const Scene& scene, vector<Mat>& films, vector<Mat>& biased_var, vector<Mat>& biased_mean, vector<Mat>& biased_m2, vector<Mat>& sumweight, unsigned long seed, bool firstIteration)
{
    renderStep(size, scene, Mat::ones(size, CV_32F), films, biased_var, biased_mean, biased_m2, sumweight, seed, firstIteration);
}

void MetropolisFltRenderer::renderStep(Size size, const Scene& scene, Mat importanceMap, vector<Mat>& films, vector<Mat>& biased_var, vector<Mat>& biased_mean, vector<Mat>& biased_m2, vector<Mat>& sumweight, unsigned long seed, bool firstIteration)
{
  const int numInitialSamples = vm["metflt-bootstrap"].as<int>();
  const float largeStepProb = vm["metflt-large-step-prob"].as<float>();
  const int numPixelSamples = vm["metflt-mutations"].as<int>();

  gsl_rng *globalrng = gsl_rng_alloc(gsl_rng_taus2);
  gsl_rng_set(globalrng, seed);

  MetropolisSample initialSample(scene.light.size());
  initialSample.largeStep(globalrng);
  UniDiPathTracingIntegrator integrator;

  const int num_films = films.size();

  float b;
  if(firstIteration)
  {
      float sumI = 0;
      vector<float> bootstrapI;
      vector<MetropolisSample> bootstrapSamples;
      for(int i = 0; i < numInitialSamples; i++)
      {
        initialSample.largeStep(globalrng);
        Path initialPath = initialSample.cameraPathFromSample(*scene.object, scene.camera);
        Vec3f l = integrator.integrate(initialPath, *scene.object, scene.light, initialSample.lightSample1, initialSample.lightIndex);
        float v = norm(l) * importanceMap.at<float>(getPos(initialSample.cameraSample, size));
        sumI += v;
        bootstrapI.push_back(v);
        bootstrapSamples.push_back(initialSample);
      }
      b = sumI / numInitialSamples;
      const float contribOffset = gsl_rng_uniform(globalrng) * sumI;
      sumI = 0;
      for(int i = 0; i < numInitialSamples; i++)
      {
        initialSample.largeStep(globalrng);
        initialSample = bootstrapSamples[i];
        sumI += bootstrapI[i];
        if(sumI > contribOffset) break;
      }
  }
  else
  {
      b = sum(channelNorm(films[0]).mul(importanceMap))[0] / films[0].size().area();
  }


  Mat newSumImportance = sumImportance + importanceMap;

  for(int n = 0; n < num_films; n++)
  {
    films[n] = extend(sumImportance / newSumImportance).mul(films[n]);
    biased_m2[n] = extend(sumImportance / newSumImportance).mul(extend(sumImportance / newSumImportance).mul(biased_m2[n]));
  }

  const int numSamples = numPixelSamples * size.area() / numThreads / vm["metflt-num-passes"].as<int>();

#pragma omp parallel for
  for(int t = 0; t < numThreads; t++)
  {
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus2);
    gsl_rng_set(rng, numThreads * seed + t);
    for(int n = 0; n < num_films; n++)
    {
      MetropolisSample currentSample = initialSample;
      Vec3f currentValue = integrator.integrate(initialSample.cameraPathFromSample(*scene.object, scene.camera), *scene.object, scene.light, initialSample.lightSample1, initialSample.lightIndex);
      Point currentPos = getPos(currentSample.cameraSample, size);

      for(int i = 0; i < numSamples; i++)
      {
          MetropolisSample newSample = currentSample.mutated(rng, largeStepProb);
          Point newPos = getPos(newSample.cameraSample, size);
          Vec3f newValue = integrator.integrate(newSample.cameraPathFromSample(*scene.object, scene.camera), *scene.object, scene.light, newSample.lightSample1, newSample.lightIndex);
          float accept = min(1., norm(newValue) * importanceMap.at<float>(newPos) / (norm(currentValue) * importanceMap.at<float>(currentPos)));
          assert(!isnan(accept));

          if(norm(currentValue) > 0 && accept < 1)
          {
              addSample(currentSample.cameraSample, (1 - accept) / newSumImportance.at<float>(currentPos), films[n], biased_mean[n], biased_m2[n], sumweight[n], currentValue * (1 / norm(currentValue) * b * vm["metflt-num-passes"].as<int>() / numPixelSamples));
          }
          if(norm(newValue) > 0 && accept > 0)
          {
              addSample(newSample.cameraSample, accept / newSumImportance.at<float>(newPos), films[n], biased_mean[n], biased_m2[n], sumweight[n], newValue * (1 / norm(newValue) * b * vm["metflt-num-passes"].as<int>() / numPixelSamples));
          }
          if(gsl_rng_uniform(rng) < accept)
          {
              currentSample = newSample;
              currentValue = newValue;
              currentPos = newPos;
          }
      }
    }
    gsl_rng_free(rng);
  }

#pragma omp parallel for
  for(int n = 0; n < num_films; n++)
  {
      biased_var[n] = biased_m2[n] / extend(sumweight[n] + 1e-8);
  }

  sumImportance = newSumImportance;
}

void MetropolisFltRenderer::run()
{
  if(doStop) return;
  this->vm = vm;
  const int seed = getSeed(vm);
  numPasses = vm["metflt-num-passes"].as<int>();

  sumImportance = Mat::zeros(film->size(), CV_32F);

  QTime time;
  time.start();

  if(vm.count("verbose"))
  {
    cout << "Starting initial rendering" << endl;
  }

  const int num_films = 2;
  vector<Mat> biased_mean(num_films), biased_m2(num_films);
  vector<Mat> films(num_films), biased_var(num_films);
  vector<Mat> sumweight(num_films);

  for(int n = 0; n < num_films; n++)
  {
    films[n] = Mat::zeros(film->size(), CV_32FC3);
    biased_mean[n] = Mat::zeros(film->size(), CV_32FC3);
    biased_m2[n] = Mat::zeros(film->size(), CV_32FC3);
    sumweight[n] = Mat::zeros(film->size(), CV_32F);
  }

  renderStep(film->size(), scene, films, biased_var, biased_mean, biased_m2, sumweight, seed, true);

  if(vm.count("verbose"))
  {
    cout << "Initial rendering complete, " << time.elapsed() / 1000 << "s elapsed, starting filtering" << endl;
  }

  Mat noisy_variance;
  var(*film, noisy_variance, films);

  assert(checkRange(noisy_variance));

  Mat meanvar, varvar;
  var(meanvar, varvar, biased_var);
  assert(checkRange(meanvar));
  assert(checkRange(varvar));

  SymmetricFilter f;
  SymmetricFilter varF(1, 2);

  Mat filteredVar;
  if(vm.count("metflt-omit-filter") || vm.count("metflt-omit-variance-filter"))
  {
      filteredVar = noisy_variance;
  }
  else
  {
      filteredVar = varF.filter(noisy_variance, meanvar, varvar);
  }

  imwrite("/tmp/film0.exr", films[0]);
  imwrite("/tmp/film1.exr", films[1]);
  imwrite("/tmp/film.exr", *film);
  imwrite("/tmp/var.exr", filteredVar);

  vector<Mat> filteredFilms(num_films);
  Mat varOfFiltered;
  if(vm.count("metflt-omit-filter"))
  {
      filteredFilms = films;
  }
  else
  {
    filteredFilms[0] = f.filter(films[0], films[1], filteredVar);
    filteredFilms[1] = f.filter(films[1], films[0], filteredVar);
  }
  Mat filteredMean;
  var(filteredMean, varOfFiltered, filteredFilms);
  if(!vm.count("metflt-omit-filter"))
  {
      *film = filteredMean;
  }

  Mat newOut;

  for(int i = 1; i < vm["metflt-num-passes"].as<int>(); i++)
  {
    if(vm.count("verbose"))
    {
        cout << time.elapsed() / 1000 << "s elapsed, starting rendering pass " << i << "/" << (vm["metflt-num-passes"].as<int>() - 1) << endl;
    }

    Mat importanceMap = channelMean(varOfFiltered) / (channelMean(filteredMean).mul(channelMean(filteredMean)) + 1e-2) + 1e-8;

    //Normalization needed for correct computation of weighted average
    importanceMap *= importanceMap.size().area() / sum(importanceMap)[0];
    QString fn = QString("/tmp/importance%1.exr").arg(i);
    imwrite(fn.toStdString(), importanceMap);
    renderStep(film->size(), scene, importanceMap, films, biased_var, biased_mean, biased_m2, sumweight, seed + i, false);
    if(vm.count("verbose"))
    {
      cout << time.elapsed() / 1000 << "s elapsed, starting filtering pass " << i << "/" << (vm["metflt-num-passes"].as<int>() - 1) << endl;
    }
    var(newOut, noisy_variance, films);
    var(meanvar, varvar, biased_var);
    if(vm.count("metflt-omit-filter") || vm.count("metflt-omit-variance-filter"))
    {
        filteredVar = noisy_variance;
    }
    else
    {
        filteredVar = f.filter(noisy_variance, meanvar, varvar);
    }
    if(vm.count("metflt-omit-filter"))
    {
        filteredFilms = films;
    }
    else
    {
      filteredFilms[0] = f.filter(films[0], films[1], filteredVar);
      filteredFilms[1] = f.filter(films[1], films[0], filteredVar);
    }
    Mat filteredMean;
    var(filteredMean, varOfFiltered, filteredFilms);

    if(vm.count("metflt-omit-filter"))
    {
        *film = newOut;
    }
    else
    {
        *film = filteredMean;
    }
    fn = QString("/tmp/iteration%1.exr").arg(i);
    imwrite(fn.toStdString(), *film);
  }
}

options_description MetropolisFltRenderer::options()
{
  options_description opts("Metropolis-Filter renderer options");
  opts.add_options()
      ("metflt-large-step-prob", value<float>()->default_value(0.1f, "0.1"), "the probability for a mutation to be a large step mutation")
      ("metflt-bootstrap", value<int>()->default_value(1000), "number of bootstrapping samples")
      ("metflt-mutations", value<int>()->default_value(16), "average number of path mutations per pixel")
      ("metflt-omit-filter", "don't filter the image")
      ("metflt-omit-variance-filter", "filter the image using unfiltered guidance")
      ("metflt-num-passes", value<int>()->default_value(3), "the number of filtering passes");
  return opts;
}

const std::string MetropolisFltRenderer::name = "metropolisflt";
