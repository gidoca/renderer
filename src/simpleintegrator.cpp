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
#include "simpleintegrator.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "jitteredsampler.h"
#include "light.h"
#include "renderer.h"

#include <gsl/gsl_rng.h>

using namespace cv;

Vec3f SimpleIntegrator::integrate(const Ray & ray, const Intersectable & scene, std::vector<Light*> light, int depth, gsl_rng *rng) const
{
  JitteredSampler sampler(2, 2, rng);
  if(depth > MAX_DEPTH) return Vec3f();

  HitRecord hit = scene.intersect(ray);
  if(hit.getMaterial().isMirror())
  {
    QVector3D oldRayDirection = hit.getRay().getDirection();
    QVector3D surfaceNormal = hit.getSurfaceNormal();
    surfaceNormal.normalize();
    QVector3D newDirection = oldRayDirection - 2 * QVector3D::dotProduct(oldRayDirection, surfaceNormal) * surfaceNormal;
    Ray mirrorRay(hit.getIntersectingPoint(), newDirection);
    return integrate(mirrorRay, scene, light, depth + 1, rng);
  }
  else
  {
    if(hit.intersects())
    {
    Vec3f result;
    QVector3D direction;
    std::list<Sample> samples = sampler.getSamples();
    for(std::list<Sample>::iterator i = samples.begin(); i != samples.end(); i++)
    {
      int lightIndex = gsl_rng_uniform_int(rng, light.size());
      Vec3f lightIntensity = light[lightIndex]->getIntensity(hit.getIntersectingPoint(), direction, scene, *i);
      Vec3f shade = hit.getMaterial().shade(hit, direction);
      result += shade.mul(lightIntensity);
    }
    return result * (1.f / samples.size());
  }
    else
    {
//      QVector3D dir = ray.getDirection();
//      return light.getIntensity(hit, dir, scene, QPointF());
      return Vec3f();
    }
  }
}
