#ifndef METROPOLISRENDERER_H
#define METROPOLISRENDERER_H

#include "global.h"
#include "renderer.h"
#include "film.h"

#include <gsl/gsl_rng.h>

#include <vector>

class MetropolisRenderer : public Renderer
{
public:
  MetropolisRenderer(QSize size): film(size)
  {
    
  }
  
  void render(const Intersectable & scene, const Camera & camera, std::vector<Light*> lights, Film & film);
  
private:
  Film film;

  Path cameraPathFromSample(MetropolisSample sample, const Intersectable & scene, const Camera & camera);
};

class MetropolisSample
{
public:
  MetropolisSample(int numLights) : numLights(numLights) {}
  void largeStep(gsl_rng *rng);
  void smallStep(gsl_rng *rng);

  Sample lightSample1[MAX_DEPTH], lightSample2[MAX_DEPTH];
  Sample cameraSample;
  Sample lightPathSamples[MAX_DEPTH];
  Sample cameraPathSamples[MAX_DEPTH];

  unsigned long int lightIndex[MAX_DEPTH];

  const int numLights;
};

#endif // METROPOLISRENDERER_H
