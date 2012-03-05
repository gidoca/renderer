#ifndef METROPOLISSAMPLE_H
#define METROPOLISSAMPLE_H

#include "global.h"
#include "renderer.h"

#include <gsl/gsl_rng.h>

#include <QPointF>

class MetropolisSample
{
public:
  MetropolisSample(int numLights) : numLights(numLights) {}

  void largeStep(gsl_rng *rng);
  void smallStep(gsl_rng *rng);
  void initAtPixel(QPointF pixel, gsl_rng *rng);
  MetropolisSample mutated(gsl_rng *rng, float largeStepProb);
  Path cameraPathFromSample(const Intersectable & scene, const Camera & camera);

  Sample lightSample1[MAX_DEPTH], lightSample2[MAX_DEPTH];
  Sample cameraSample;
  Sample lightPathSamples[MAX_DEPTH];
  Sample cameraPathSamples[MAX_DEPTH];

  unsigned long int lightIndex[MAX_DEPTH];

  int numLights;
};

#endif // METROPOLISSAMPLE_H
