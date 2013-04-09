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
#include "conelight.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "sampler.h"

#include <cmath>

using namespace cv;

Vec3f ConeLight::getIntensity(const QVector3D & at, QVector3D &direction, const Intersectable &scene, const Sample &) const
{
  direction = at - location;
  HitRecord shadowHit = scene.intersect(Ray(at, -direction.normalized(), EPSILON, direction.length()));
  cv::Vec3f part = cv::Vec3f(1, 1, 1);
  float rayAngle = QVector3D::dotProduct(direction.normalized(), this->direction.normalized());
  if(0 <= acos(rayAngle) && acos(rayAngle) <= openingAngle && !shadowHit.intersects())
  {
    return intensity.mul(part) * (1 / direction.lengthSquared());
  }
  else
  {
    return Vec3f();
  }
}

Ray ConeLight::getRandomRay(const Sample &sample, const Sample&, float &pdf) const
{
  return Ray(location, sample.getCosineWeightedDirection(direction, pdf, openingAngle));
}
