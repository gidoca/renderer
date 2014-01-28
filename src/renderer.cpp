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
#include "renderer.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "jitteredsampler.h"
#include "transparentmaterial.h"

#include "energyredistributionrenderer.h"
#include "perpixelrenderer.h"
#include "metropolisrenderer.h"
#include "metropolisfltrenderer.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

#include <QTime>

#include <opencv2/core/core.hpp>

using namespace std;

Path Renderer::createPath(const Ray &primaryRay, const Intersectable &scene, gsl_rng *rng, cv::Vec3f initialAlpha, float terminationProb)
{
    JitteredSampler sampler(1, 1, rng);
    Sample pathSamples[MAX_DEPTH];
    int pathLength;
    if(terminationProb > 0)
    {
      pathLength = min<int>(1 + gsl_ran_negative_binomial(rng, terminationProb, 1), MAX_DEPTH);
    }
    else
    {
      terminationProb = 1;
      pathLength = MAX_DEPTH;
    }
    for(int i = 0; i < MAX_DEPTH; i++)
    {
      pathSamples[i] = sampler.getSamples().front();
    }
    return createPath(primaryRay, scene, pathSamples, initialAlpha, pathLength, terminationProb);
}

Path Renderer::createPath(const Ray& primaryRay, const Intersectable &scene, const Sample pathSamples[], cv::Vec3f alpha, int pathLength, float russianRoulettePdf)
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
  for(int i = 0; i < pathLength; i++)
  {
    if(!hit.intersects()) return result;


    float pdf;
    cv::Vec3f brdf;
    QVector3D outDirection = hit.getMaterial().outDirection(hit, pathSamples[i], pdf, brdf);
    if(hit.getMaterial().emitsLight())
    {
        result.alphaValues.push_back(alpha);
        result.hitRecords.push_back(hit);
        return result;
    }
    else if(hit.getMaterial().isSpecular())
    {
        //Try not to terminate on refractive vertices
        if(i == pathLength - 1 && pathLength < MAX_DEPTH) pathLength++;
    }
    else
    {
      result.alphaValues.push_back(alpha);
      result.hitRecords.push_back(hit);
      if(pdf == 0) return result;
      float cos = QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized());
      assert(cos >= 0 && !isnan(pdf));
      assert(pdf > 0 && !isnan(pdf));
      alpha = alpha.mul(brdf) * (cos / pdf / russianRoulettePdf);
    }
    hit = scene.intersect(Ray(hit.getIntersectingPoint(), outDirection));
  }
  return result;
}

boost::program_options::options_description Renderer::options()
{
  return boost::program_options::options_description("Renderer");
}

unsigned long Renderer::getSeed()
{
  if(vm.count("fixed-seed") || vm.count("seed"))
  {
      return vm["seed"].as<unsigned long>();
  }
  else
  {
    return (unsigned long)QTime::currentTime().msec() + 1000ul * (unsigned long)QTime::currentTime().second();
  }
}

void Renderer::stopRendering()
{
    doStop = true;
    wait();
}

void Renderer::prepareRenderingScene(Scene scene)
{
    stopRendering();
    doStop = false;
    this->scene = scene;
    film->setTo(cv::Vec3f(0, 0, 0));
}

void Renderer::run()
{
    if(doStop) return;
    render();
    if(doStop) return;
    Q_EMIT finishedRendering();
}
