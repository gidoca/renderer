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

private:
  void renderStep(cv::Size size, const Scene &scene, cv::Mat importanceMap, std::vector<cv::Mat> &films, std::vector<cv::Mat> &biased_var, std::vector<cv::Mat> &biased_mean, std::vector<cv::Mat> &biased_m2);
  void renderStep(cv::Size size, const Scene &scene, std::vector<cv::Mat> &films, std::vector<cv::Mat> &biased_var, std::vector<cv::Mat> &biased_mean, std::vector<cv::Mat> &biased_m2);

  const int numThreads;
  int numPasses;

  boost::program_options::variables_map vm;
};

#endif // METROPOLISFLTRENDERER_H
