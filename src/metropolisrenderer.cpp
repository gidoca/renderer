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

#include <omp.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace boost::program_options;
using namespace cv;

void addSample(const Sample &cameraSample, float weight, Mat &film, Vec3f value)
{
    int x = (int)(cameraSample.getSample().x() * film.size().width);
    int y = (int)(cameraSample.getSample().y() * film.size().height);
    assert(0 <= x && x < film.size().width);
    assert(0 <= y && y < film.size().height);

    Vec3f depositValue = weight * value;

#pragma omp critical
    film.at<Vec3f>(y, x) += depositValue;
}

void MetropolisRenderer::render(const Scene & scene, Mat & film, const boost::program_options::variables_map vm)
{
    Mat importanceMap(film.size(), CV_32F);
//    for(int i = 0; i < film.size().height; i++)
//    {
//        Mat importanceMapHalf = importanceMap(Range(i, i + 1), Range::all());
//        importanceMapHalf.setTo(((i / 50) & 0x1) + 10.5);
//    }
    importanceMap = Mat::ones(film.size(), CV_32F);

    importanceMap *= importanceMap.size().area() / sum(importanceMap)[0];

  gsl_rng *globalrng = gsl_rng_alloc(gsl_rng_taus2);
  const unsigned long seed = getSeed(vm);
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
//    int currentImageX = (int)(sample.cameraSample.getSample().x() * film.size().width);
//    int currentImageY = (int)(sample.cameraSample.getSample().y() * film.size().height);
//    float currentImportance = importanceMap.at<float>(currentImageY, currentImageX);

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
//  sample.largeStep(globalrng);

  gsl_rng_free(globalrng);

  const int numPixelSamples = vm["met-mutations"].as<int>();
#ifdef NDEBUG
  const int numThreads = omp_get_max_threads();
#else
  const int numThreads = 1;
#endif
  const int numSamples = numPixelSamples * film.size().width * film.size().height / numThreads;

  Mat virtualSamples = Mat::zeros(film.size(), CV_32F);
  Mat realSamples = Mat::zeros(film.size(), CV_32F);

#pragma omp parallel for
  for(int t = 0; t < numThreads; t++)
  {
        gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus2);
        gsl_rng_set(rng, numThreads * seed + t);
        MetropolisSample currentSample = sample;
        Vec3f currentValue = value;
        Path currentPath = path;

        int currentImageX = (int)(currentSample.cameraSample.getSample().x() * film.size().width);
        int currentImageY = (int)(currentSample.cameraSample.getSample().y() * film.size().height);
        float currentImportance = importanceMap.at<float>(currentImageY, currentImageX);

        for(int i = 0; i < numSamples; i++)
        {
            MetropolisSample newSample = currentSample.mutated(rng, largeStepProb);
            currentPath = newSample.cameraPathFromSample(*scene.object, scene.camera);
            Vec3f newValue = integrator.integrate(currentPath, *scene.object, scene.light, newSample.lightSample1, newSample.lightIndex);
            int newImageX = (int)(newSample.cameraSample.getSample().x() * film.size().width);
            int newImageY = (int)(newSample.cameraSample.getSample().y() * film.size().height);
            float newImportance = importanceMap.at<float>(newImageY, newImageX);

            float accept = min(1., norm(newValue) * newImportance / (norm(currentValue) * currentImportance));
            assert(!isnan(accept));

            if(norm(currentValue) > 0)
            {
                addSample(currentSample.cameraSample, 1 - accept, film, currentValue * (1.f / norm(currentValue) * b / numPixelSamples));
                virtualSamples.at<float>(currentImageY, currentImageX) += (1 - accept) / currentImportance;
                realSamples.at<float>(currentImageY, currentImageX) += 1 - accept;
            }
            if(norm(newValue) > 0)
            {
                addSample(newSample.cameraSample, accept, film, newValue * (1.f / norm(newValue) * b / numPixelSamples));
                virtualSamples.at<float>(newImageY, newImageX) += accept / newImportance;
                realSamples.at<float>(newImageY, newImageX) += accept;
            }
            if(gsl_rng_uniform(rng) < accept)
            {
                currentSample = newSample;
                currentValue = newValue;
                currentImageX = newImageX;
                currentImageY = newImageY;
                currentImportance = newImportance;
            }
        }
        gsl_rng_free(rng);
  }
  film = film.mul(extend(virtualSamples / realSamples));
}

options_description MetropolisRenderer::options()
{
  options_description opts("Metropolis renderer options");
  opts.add_options()
      ("met-large-step-prob", value<float>()->default_value(0.1f, "0.1"), "the probability for a mutation to be a large step mutation")
      ("met-bootstrap", value<int>()->default_value(1000), "number of bootstrapping samples")
      ("met-mutations", value<int>()->default_value(16), "average number of path mutations per pixel");
  return opts;
}

const std::string MetropolisRenderer::name = "metropolis";
