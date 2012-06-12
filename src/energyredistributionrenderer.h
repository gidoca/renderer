#ifndef ENERGYREDISTRIBUTIONRENDERER_H
#define ENERGYREDISTRIBUTIONRENDERER_H

#include "global.h"
#include "renderer.h"
#include "metropolissample.h"

#include <gsl/gsl_rng.h>

#include <opencv2/core/core.hpp>

#include <vector>
#include <string>

class EnergyRedistributionRenderer: public Renderer
{
public:
  void render(const Scene & scene, cv::Mat & film, const boost::program_options::variables_map vm);
	
	static boost::program_options::options_description options();

  static const std::string name;

private:
  float computeEd(const Scene & scene, gsl_rng *rng, int pathsPerPixel);
  void equalDispositionFlow(cv::Mat & film, MetropolisSample sample, const Intersectable& scene, const std::vector<const Light*> light, const Camera & camera, gsl_rng *rng, float ed, boost::program_options::variables_map vm);
};

#endif // ENERGYREDISTRIBUTIONRENDERER_H
