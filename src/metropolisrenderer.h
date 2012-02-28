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
  void render(const Intersectable & scene, const Camera & camera, std::vector<Light*> lights, Film & film, boost::program_options::variables_map vm);

  boost::program_options::options_description options() const;
  
private:
  Path cameraPathFromSample(MetropolisSample sample, const Intersectable & scene, const Camera & camera);

};

class MetropolisSample
{
public:
  MetropolisSample(int numLights) : numLights(numLights) {}
  void largeStep(gsl_rng *rng);
  void smallStep(gsl_rng *rng);
  MetropolisSample mutated(gsl_rng *rng, float largeStepProb);

  Sample lightSample1[MAX_DEPTH], lightSample2[MAX_DEPTH];
  Sample cameraSample;
  Sample lightPathSamples[MAX_DEPTH];
  Sample cameraPathSamples[MAX_DEPTH];

  unsigned long int lightIndex[MAX_DEPTH];

  int numLights;
};

#endif // METROPOLISRENDERER_H
