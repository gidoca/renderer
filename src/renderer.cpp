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

#include <QTime>

#include <opencv2/core/core.hpp>

using namespace std;

struct initializer
{
  Renderer **renderer;
  std::string name;
  initializer(Renderer **renderer, std::string name) : renderer(renderer), name(name) {}

  template< typename R > void operator()(R)
  {
    if(name == R::name && *renderer == nullptr)
    {
      *renderer = new R();
    }
  }
};

Renderer *getRendererByName(string name)
{
  Renderer * renderer = nullptr;
  boost::mpl::for_each<renderers>(initializer(&renderer, name));
  return renderer;
}

Path Renderer::createPath(const Ray &primaryRay, const Intersectable &scene, gsl_rng *rng, cv::Vec3f initialAlpha, float terminationProb)
{
    JitteredSampler sampler(1, 1, rng);
    Sample pathSamples[MAX_DEPTH];
    int pathLength;
    if(terminationProb > 0)
    {
      pathLength = max(1, min<int>(gsl_ran_negative_binomial(rng, terminationProb, 1), MAX_DEPTH));
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

Path Renderer::createPath(const Ray& primaryRay, const Intersectable &scene, Sample pathSamples[], cv::Vec3f alpha, int pathLength, float russianRoulettePdf)
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
  for(int i = 0; i < pathLength; i++)
  {
    if(!hit.intersects()) return result;

    result.alphaValues.push_back(alpha);
    result.hitRecords.push_back(hit);

    float pdf;
    QVector3D outDirection;
    if(hit.getMaterial().isMirror())
    {
      QVector3D oldRayDirection = hit.getRay().getDirection();
      QVector3D surfaceNormal = hit.getSurfaceNormal();
      surfaceNormal.normalize();
      outDirection = oldRayDirection - 2 * QVector3D::dotProduct(oldRayDirection, surfaceNormal) * surfaceNormal;
    }
    else if(hit.getMaterial().refractive())
    {
        const TransparentMaterial* transparentMaterial = hit.getMaterial().refractive();
        outDirection = transparentMaterial->outDirection(hit.getRay().getDirection(), hit.getSurfaceNormal());
        //Try not to terminate on refractive vertices
        if(i == pathLength - 1 && pathLength < MAX_DEPTH) pathLength++;
    }
    else
    {
      outDirection = pathSamples[i].getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
      if(pdf == 0) return result;
      cv::Vec3f brdf = hit.getMaterial().shade(hit, -outDirection);
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

unsigned long Renderer::getSeed(boost::program_options::variables_map vm)
{
  if(vm.count("fixed-seed"))
  {
      return vm["seed"].as<unsigned long>();
  }
  else
  {
    return QTime::currentTime().msec() + 1000ul * QTime::currentTime().second();
  }
}
