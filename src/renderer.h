#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>

#include <opencv2/core/core.hpp>

#include "global.h"
#include "scene.h"
#include "path.h"
#include "sampler.h"

#define MAX_DEPTH 4

typedef boost::mpl::list<MetropolisRenderer, MetropolisFltRenderer, PerPixelRenderer, EnergyRedistributionRenderer> renderers;

class Renderer
{
public:
  virtual ~Renderer() {}
  virtual void render(const Scene & scene, cv::Mat & film, const boost::program_options::variables_map vm) = 0;
  
  static boost::program_options::options_description options();

  static Path createPath(const Ray & primaryRay, const Intersectable & scene, gsl_rng *rng, cv::Vec3f initialAlpha = cv::Vec3f(1, 1, 1), float terminationProb = 0.f);
  static Path createPath(const Ray & primaryRay, const Intersectable & scene, Sample pathSamples[], cv::Vec3f initialAlpha = cv::Vec3f(1, 1, 1), int pathLength = MAX_DEPTH, float russianRoulettePdf = 1);

protected:
  int getSeed(boost::program_options::variables_map vm);
};

Renderer *getRendererByName(std::string name);

#endif // RENDERER_H
