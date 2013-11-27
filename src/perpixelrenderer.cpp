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
#include "perpixelrenderer.h"

#include "intersectable.h"
#include "camera.h"
#include "light.h"
#include "unidipathtracingintegrator.h"
#include "bidipathtracingintegrator.h"
#include "jitteredsampler.h"
#include "integrator.h"

#include <iostream>
#include <cassert>
#include <cmath>

#include <QTime>

#include <gsl/gsl_rng.h>

using namespace boost::program_options;
using namespace std;
using namespace cv;

void PerPixelRenderer::render()
{
  Integrator * integrator;
  if(vm["pt-integrator"].as<string>() == "unidi")
  {
    integrator = new UniDiPathTracingIntegrator(vm["pt-termination-prob"].as<float>());
  }
  else
  {
    integrator = new BiDiPathTracingIntegrator();
  }

  QTime time;
  time.start();

  unsigned long seed = getSeed();

  cv::Vec3f sumI;
  
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < film->size().height; i++)
  {
    if(doStop) continue;

    gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus2);
    gsl_rng_set(rng, film->size().height * seed + i);
    if(vm.count("verbose"))
    {
      std::cerr << i * 100 / film->size().height << "% complete, ETA: " << (long)time.elapsed() * (film->size().height - i) / ((i + 1) * 1000) << "s" << std::endl;
    }
    Vec3f * scanline = film->ptr<Vec3f>(i);
    for(int j = 0; j < film->size().width; j++)
    {
      if(doStop) continue;

      JitteredSampler multiSampler(vm["pt-x-samples"].as<int>(), vm["pt-y-samples"].as<int>(), rng);
      std::list<Sample> samples = multiSampler.getSamples();
      QPointF point = QPoint(j, i);
      for(std::list<Sample>::iterator it = samples.begin(); it != samples.end(); it++)
      {
        QPointF samplePoint = point + it->getSample();
        Ray ray = scene.camera.getRay(samplePoint);
        if(j == 569 && i == 300)
        {
            std::cerr << "f" << std::endl;
        }
        Vec3f s = integrator->integrate(ray, *scene.object, scene.light, rng);
        assert(!isnan(s[0]) && !isnan(s[1]) && !isnan(s[2]));
        assert(s[0] >= 0 && s[1] >= 0 && s[2] >= 0);
        scanline[j] += s * (1.f / samples.size());
#pragma omp critical
        sumI += s * (1.f / samples.size());
      }
    }
    gsl_rng_free(rng);
  }

  std::cerr << "Luminance: " << sumI[2] / film->size().area() << " " << sumI[1] / film->size().area() << " " << sumI[0] / film->size().area() << " " << std::endl;
}

options_description PerPixelRenderer::options()
{
  options_description opts("Path tracer options");
  opts.add_options()
      ("pt-integrator", value<string>()->default_value("unidi"), "the path tracing method (unidi or bidi)")
      ("pt-termination-prob", value<float>()->default_value(0.5f, "0.5"), "the roussian roulette path termination probability (use 0 to disable roussian roulette path termination)")
      ("pt-x-samples", value<int>()->default_value(4), "number of samples per pixel in x direction")
      ("pt-y-samples", value<int>()->default_value(4), "number of samples per pixel in y direction");
  return opts;
}

const std::string PerPixelRenderer::name = "pathtracing";
