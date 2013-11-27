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
#ifndef METROPOLISSAMPLE_H
#define METROPOLISSAMPLE_H

#include "global.h"
#include "renderer.h"

#include <gsl/gsl_rng.h>

#include <QPointF>

class MetropolisSample
{
public:
  MetropolisSample(int numLights) : numLights(numLights), terminationProb(1), pathLength(MAX_DEPTH) {}

  void largeStep(gsl_rng *rng);
  void smallStep(gsl_rng *rng);
  void mutatePathLength(gsl_rng *rng, float terminationProb);
  void initAtPixel(QPointF pixel, gsl_rng *rng);
  MetropolisSample mutated(gsl_rng *rng, float largeStepProb, float terminationProb);
  Path cameraPathFromSample(const Intersectable & scene, const Camera & camera);

  Sample lightSample1[MAX_DEPTH], lightSample2[MAX_DEPTH];
  Sample cameraSample;
  Sample lightPathSamples[MAX_DEPTH];
  Sample cameraPathSamples[MAX_DEPTH];

  unsigned long int lightIndex[MAX_DEPTH];

  int numLights;

  float terminationProb;
  int pathLength;
};

#endif // METROPOLISSAMPLE_H
