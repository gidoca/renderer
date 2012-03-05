#ifndef ENERGYREDISTRIBUTIONRENDERER_H
#define ENERGYREDISTRIBUTIONRENDERER_H

#include "global.h"
#include "renderer.h"
#include "metropolissample.h"

#include <gsl/gsl_rng.h>

#include <vector>

class EnergyRedistributionRenderer: public Renderer
{
public:
  void render(const Scene & scene, Film & film, const boost::program_options::variables_map vm);

private:
  float computeEd(const Scene & scene, gsl_rng *rng, int pathsPerPixel);
  void equalDispositionFlow(Film & film, MetropolisSample sample, const Intersectable& scene, const std::vector<const Light*> light, const Camera & camera, gsl_rng *rng, float ed);
};

#endif // ENERGYREDISTRIBUTIONRENDERER_H
