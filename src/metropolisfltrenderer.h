#ifndef METROPOLISFLTRENDERER_H
#define METROPOLISFLTRENDERER_H

#include "global.h"
#include "renderer.h"

#include <gsl/gsl_rng.h>

#include <opencv2/core/core.hpp>

#include <vector>
#include <string>

class MetropolisFltRenderer : public Renderer
{
public:
  MetropolisFltRenderer();

  void render(const Scene & scene, cv::Mat & film, const boost::program_options::variables_map vm);

  static boost::program_options::options_description options();
  
  static const std::string name;

  const int numThreads;
};

#endif // METROPOLISFLTRENDERER_H
