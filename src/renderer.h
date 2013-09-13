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

#include <QThread>

#include <vector>
#include <string>

#include <boost/program_options.hpp>

#include <opencv2/core/core.hpp>

#include "global.h"
#include "scene.h"
#include "path.h"
#include "sampler.h"

#define MAX_DEPTH 6

class Renderer : public QThread
{
  Q_OBJECT

public:
  Renderer() : doStop(true) {}
  Renderer(const Renderer& other) : QThread(), doStop(other.doStop), scene(other.scene), film(other.film), vm(other.vm) {}
  virtual ~Renderer() {}

  void setOutput(cv::Mat * film)
  {
      this->film = film;
  }
  void setOptions(const boost::program_options::variables_map vm)
  {
      this->vm = vm;
  }

  void prepareRenderingScene(Scene scene);
  void stopRendering();
  
  static boost::program_options::options_description options();

  static Path createPath(const Ray & primaryRay, const Intersectable & scene, gsl_rng *rng, cv::Vec3f initialAlpha = cv::Vec3f(1, 1, 1), float terminationProb = 0.f);
  static Path createPath(const Ray & primaryRay, const Intersectable & scene, Sample pathSamples[], cv::Vec3f initialAlpha = cv::Vec3f(1, 1, 1), int pathLength = MAX_DEPTH, float russianRoulettePdf = 1);

Q_SIGNALS:
  void finishedRendering();

protected:
  unsigned long getSeed(boost::program_options::variables_map vm);

  void run();
  virtual void render() = 0;

  bool doStop;
  Scene scene;
  cv::Mat * film;
  boost::program_options::variables_map vm;
};

#endif // RENDERER_H
