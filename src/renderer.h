#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "global.h"
#include "film.h"
#include "path.h"
#include "spectrum.h"
#include "sampler.h"

#define MAX_DEPTH 4

class Renderer
{
public:
  virtual void render(const Intersectable & scene, const Camera & camera, std::vector<Light*> lights, Film & film) = 0;
  
  static Path createPath(const Ray& primaryRay, const Intersectable& scene, gsl_rng *rng, Spectrum initialAlpha = Spectrum(1, 1, 1));
  static Path createPath(const Ray &primaryRay, const Intersectable &scene, Sample pathSamples[], Spectrum initialAlpha = Spectrum(1, 1, 1));
};

#endif // RENDERER_H
