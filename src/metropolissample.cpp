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
#include "metropolissample.h"

#include "jitteredsampler.h"

#include <gsl/gsl_randist.h>

#include <cmath>
#include <algorithm>

void MetropolisSample::mutatePathLength(gsl_rng *rng, float terminationProb)
{
    if(terminationProb > 0)
    {
      this->terminationProb = terminationProb;
      pathLength = 1 + std::min<int>(gsl_ran_negative_binomial(rng, terminationProb, 1), MAX_DEPTH - 1);
    }
    else
    {
      this->terminationProb = 1;
      pathLength = MAX_DEPTH;
    }
}

void MetropolisSample::largeStep(gsl_rng *rng)
{
  JitteredSampler sampler(1, 1, rng);
  cameraSample = sampler.getSamples().front();

  for(int i = 0; i < MAX_DEPTH; i++)
  {
    lightPathSamples[i] = sampler.getSamples().front();
    cameraPathSamples[i] = sampler.getSamples().front();
    lightSample1[i] = sampler.getSamples().front();
    lightSample2[i] = sampler.getSamples().front();
    lightIndex[i] = gsl_rng_uniform_int(rng, numLights);
  }

}

void mutate(qreal &s, gsl_rng *rng)
{
  JitteredSampler sampler(1, 1, rng);
  Sample sample = sampler.getSamples().front();
  static const float a = 1. / 1024., b = 1. / 64.;
  static const float logRatio = -log(b / a);
  float delta = b * exp(logRatio * sample.getSample().x());
  if(sample.getSample().y() < 0.5)
  {
    delta *= -1;
  }

  s += delta;
  if(s < 0) s += 1;
  if(s > 1) s -= 1;
  assert(0 <= s && s <= 1);
}

inline void mutate(QPointF &s, gsl_rng *rng)
{
    mutate(s.rx(), rng);
    mutate(s.ry(), rng);
}

void MetropolisSample::smallStep(gsl_rng *rng)
{
  mutate(cameraSample.getSample(), rng);
  for(int i = 0; i < MAX_DEPTH; i++)
  {
      mutate(lightPathSamples[i].getSample(), rng);
      mutate(cameraPathSamples[i].getSample(), rng);
      mutate(lightSample1[i].getSample(), rng);
      mutate(lightSample2[i].getSample(), rng);
  }
}

void MetropolisSample::initAtPixel(QPointF cameraSample, gsl_rng *rng)
{
  largeStep(rng);
  this->cameraSample = Sample(cameraSample);
}

MetropolisSample MetropolisSample::mutated(gsl_rng *rng, float largeStepProb, float terminationProb) const
{
  MetropolisSample result = *this;
  result.mutatePathLength(rng, terminationProb);
  float s = gsl_rng_uniform(rng);
  if(s < largeStepProb)
  {
    result.largeStep(rng);
  }
  else
  {
    result.smallStep(rng);
  }
  return result;
}

Path MetropolisSample::cameraPathFromSample(const Intersectable & scene, const Camera& camera) const
{
  QPointF pixel = cameraSample.getSample();
  pixel.rx() *= camera.getResolution().width();
  pixel.ry() *= camera.getResolution().height();
  Path result = Renderer::createPath(camera.getRay(pixel), scene, cameraPathSamples, cv::Vec3f(1, 1, 1), pathLength, terminationProb);
  return result;
}

cv::Point2i MetropolisSample::getPos(cv::Size size) const
{
    int x = std::max(0, std::min<int>(cameraSample.getSample().x() * size.width, size.width - 1));
    int y = std::max(0, std::min<int>(cameraSample.getSample().y() * size.height, size.height - 1));
    assert(0 <= x && x < size.width);
    assert(0 <= y && y < size.height);
    return cv::Point2i(x, y);
}
