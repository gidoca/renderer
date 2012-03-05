#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <boost/program_options.hpp>

#include "global.h"
#include "scene.h"
#include "film.h"
#include "path.h"
#include "spectrum.h"
#include "sampler.h"

#define MAX_DEPTH 4

class Renderer
{
public:
  virtual void render(const Scene & scene, Film & film, const boost::program_options::variables_map vm) = 0;
  
  static boost::program_options::options_description options();

  static Path createPath(const Ray & primaryRay, const Intersectable & scene, gsl_rng *rng, Spectrum initialAlpha = Spectrum(1, 1, 1), float terminationProb = 0.f);
  static Path createPath(const Ray & primaryRay, const Intersectable & scene, Sample pathSamples[], Spectrum initialAlpha = Spectrum(1, 1, 1), int pathLength = MAX_DEPTH, float russianRoulettePdf = 1);

protected:
  int getSeed(boost::program_options::variables_map vm);
};

#endif // RENDERER_H
