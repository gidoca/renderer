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

#define MAX_DEPTH 6

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
  unsigned long getSeed(boost::program_options::variables_map vm);
};

Renderer *getRendererByName(std::string name);

#endif // RENDERER_H
