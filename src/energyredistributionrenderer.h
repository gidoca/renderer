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
  void render();
	
	static boost::program_options::options_description options();

  static const std::string name;

private:
  float computeEd(const Scene & scene, gsl_rng *rng, int pathsPerPixel);
  void equalDispositionFlow(MetropolisSample sample, const Intersectable& scene, const std::vector<const Light*> light, const Camera & camera, gsl_rng *rng, float ed);
};

#endif // ENERGYREDISTRIBUTIONRENDERER_H
