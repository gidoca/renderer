/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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

  void render();

  static boost::program_options::options_description options();
  
  static const std::string name;

private:
  void renderStep(cv::Size size, const Scene &scene, cv::Mat importanceMap, std::vector<cv::Mat> &films, std::vector<cv::Mat> &biased_var, std::vector<cv::Mat> &biased_mean, std::vector<cv::Mat> &biased_m2, std::vector<cv::Mat> &sumweight, unsigned long seed, bool firstIteration);
  void renderStep(cv::Size size, const Scene &scene, std::vector<cv::Mat> &films, std::vector<cv::Mat> &biased_var, std::vector<cv::Mat> &biased_mean, std::vector<cv::Mat> &biased_m2, std::vector<cv::Mat> &sumweight, unsigned long seed, bool firstIteration);

  void addSample(const MetropolisSample &cameraSample, float weight, cv::Mat &film, cv::Mat &mean, cv::Mat &m2, cv::Mat &sumweight, cv::Vec3f value);

  const int numThreads;
  int numPasses;

  cv::Mat sumImportance;
};

#endif // METROPOLISFLTRENDERER_H
