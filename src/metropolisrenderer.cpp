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
#include "metropolisrenderer.h"

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

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace boost::program_options;
using namespace cv;

void addSample(const MetropolisSample &sample, float weight, Mat &film, Vec3f value)
{
    Point2i pos = sample.getPos(film.size());
    Vec3f depositValue = weight * value;

#pragma omp critical
    film.at<Vec3f>(pos) += depositValue;
}

void MetropolisRenderer::render()
{
    Mat importanceMap(film->size(), CV_32F);
//    for(int i = 0; i < film.size().height; i++)
//    {
//        Mat importanceMapHalf = importanceMap(Range(i, i + 1), Range::all());
//        importanceMapHalf.setTo(((i / 50) & 0x1) + 10.5);
//    }
    importanceMap = Mat::ones(film->size(), CV_32F);

    importanceMap *= importanceMap.size().area() / sum(importanceMap)[0];

  gsl_rng *globalrng = gsl_rng_alloc(gsl_rng_taus2);
  const unsigned long seed = getSeed();
  gsl_rng_set(globalrng, seed);


  MetropolisSample sample(scene.light.size());
  sample.largeStep(globalrng);
  UniDiPathTracingIntegrator integrator;

  const int numInitialSamples = vm["met-bootstrap"].as<int>();
  const float largeStepProb = vm["met-large-step-prob"].as<float>();
  const float terminationProb = vm["met-termination-prob"].as<float>();

  float sumI = 0;
  vector<float> bootstrapI;
  vector<MetropolisSample> bootstrapSamples;
  for(int i = 0; i < numInitialSamples; i++)
  {
    sample.largeStep(globalrng);
    Path path = sample.cameraPathFromSample(*scene.object, scene.camera);
    Vec3f l = integrator.integrate(path, *scene.object, scene.light, sample.lightSample1, sample.lightIndex);
//    int currentImageX = (int)(sample.cameraSample.getSample().x() * film.size().width);
//    int currentImageY = (int)(sample.cameraSample.getSample().y() * film.size().height);
//    float currentImportance = importanceMap.at<float>(currentImageY, currentImageX);

    sumI += lum(l);
    bootstrapI.push_back(lum(l));
    bootstrapSamples.push_back(sample);
  }
  const float b = sumI / numInitialSamples;
  if(vm.count("verbose"))
  {
    std::cerr << "Luminance: " << b << std::endl;
  }

  const float contribOffset = gsl_rng_uniform(globalrng) * sumI;
  sumI = 0;
  for(int i = 0; i < numInitialSamples; i++)
  {
    //sample.largeStep(globalrng);
    sample = bootstrapSamples[i];
    sumI += bootstrapI[i];
    if(sumI > contribOffset) break;
  }
  //sample.largeStep(globalrng);
  Path path = sample.cameraPathFromSample(*scene.object, scene.camera);
  Vec3f value = integrator.integrate(path, *scene.object, scene.light, sample.lightSample1, sample.lightIndex);

  gsl_rng_free(globalrng);

  const int numPixelSamples = vm["met-mutations"].as<int>();
#ifdef USE_OPENMP
  const int numThreads = omp_get_max_threads();
#else
  const int numThreads = 1;
#endif
  const long numSamples = (long)numPixelSamples * film->size().area() / numThreads;

  //Mat virtualSamples = Mat::zeros(film->size(), CV_32F);
  //Mat realSamples = Mat::zeros(film->size(), CV_32F);

  int nacc = 0;

#pragma omp parallel for
  for(int t = 0; t < numThreads; t++)
  {
      int threadLocalNacc = 0;
        if(doStop) continue;

        Mat threadLocalFilm = Mat::zeros(film->size(), film->type());
        //Mat threadLocalVirtualSamples = Mat::zeros(virtualSamples.size(), virtualSamples.type());
        //Mat threadLocalRealSamples = Mat::zeros(virtualSamples.size(), realSamples.type());

        gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus2);
        gsl_rng_set(rng, numThreads * seed + t);
        MetropolisSample currentSample = sample;
        //sample.largeStep(rng);
        Vec3f currentValue = value;
        Path currentPath = path;

        cv::Point2i currentImagePos = currentSample.getPos(film->size());
        float currentImportance = importanceMap.at<float>(currentImagePos);

        for(long i = 0; i < numSamples; i++)
        {
            if(doStop) continue;
            MetropolisSample newSample = currentSample.mutated(rng, largeStepProb, terminationProb);
            currentPath = newSample.cameraPathFromSample(*scene.object, scene.camera);
            Vec3f newValue = integrator.integrate(currentPath, *scene.object, scene.light, newSample.lightSample1, newSample.lightIndex);

            cv::Point2i newImagePos = newSample.getPos(film->size());
            float newImportance = importanceMap.at<float>(newImagePos);

            float accept = min<float>(1., lum(newValue) * newImportance / (lum(currentValue) * currentImportance));
            assert(!isnan(accept));

            if(lum(currentValue) > 0)
            {
                addSample(currentSample, 1 - accept, threadLocalFilm, currentValue * (1.f / lum(currentValue) * b / numPixelSamples));
                //threadLocalVirtualSamples.at<float>(currentImageY, currentImageX) += (1 - accept) / currentImportance;
                //threadLocalRealSamples.at<float>(currentImageY, currentImageX) += 1 - accept;
            }
            if(lum(newValue) > 0)
            {
                addSample(newSample, accept, threadLocalFilm, newValue * (1.f / lum(newValue) * b / numPixelSamples));
                //threadLocalVirtualSamples.at<float>(newImageY, newImageX) += accept / newImportance;
                //threadLocalRealSamples.at<float>(newImageY, newImageX) += accept;
            }
            if(gsl_rng_uniform(rng) < accept)
            {
                currentSample = newSample;
                currentValue = newValue;
                currentImagePos = newImagePos;
                currentImportance = newImportance;
                threadLocalNacc++;
            }
            if(vm.count("verbose") && i % 100000 == 0)
            {
                std::cerr << "Thread " << t + 1 << " of " << numThreads << ": " << 100 * i / (numSamples - 1) << "%" << std::endl;
            }

        }
#pragma omp critical
        {
            *film += threadLocalFilm;
            //virtualSamples += threadLocalVirtualSamples;
            //realSamples += threadLocalRealSamples;
            nacc += threadLocalNacc;
        }
        gsl_rng_free(rng);
  }
  std::cerr << 100.f * nacc / numSamples / numThreads << "% accepted" << std::endl;
  //*film = film->mul(extend(virtualSamples / realSamples));
}

options_description MetropolisRenderer::options()
{
  options_description opts("Metropolis renderer options");
  opts.add_options()
      ("met-large-step-prob", value<float>()->default_value(0.1f, "0.1"), "the probability for a mutation to be a large step mutation")
      ("met-termination-prob", value<float>()->default_value(0.5f, "0.5"))
      ("met-bootstrap", value<int>()->default_value(10000), "number of bootstrapping samples")
      ("met-mutations", value<int>()->default_value(16), "average number of path mutations per pixel");
  return opts;
}

const std::string MetropolisRenderer::name = "metropolis";
