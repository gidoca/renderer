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
#include "arealight.h"

#include "path.h"
#include "hitrecord.h"
#include "intersectable.h"

#include <cassert>
#include <cmath>
#include <iostream>

AreaLight::AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, cv::Vec3f intensity) : origin(origin), uDirection(uDirection), vDirection(vDirection), intensity(intensity), normal(QVector3D::crossProduct(uDirection, vDirection))
{
}

cv::Vec3f AreaLight::getIntensity(const QVector3D & at, QVector3D &direction, const Intersectable &scene, const Sample &sample) const
{
  QPointF p = sample.getSample();
  QVector3D lightLocation = getLocation(p);
  direction = at - lightLocation;
  HitRecord shadowHit = scene.intersect(Ray(at, -direction.normalized(), EPSILON, direction.length()));
  cv::Vec3f part = cv::Vec3f(1, 1, 1);

  if(shadowHit.intersects())
  {
    return cv::Vec3f();
  }
  else
  {
    float c = QVector3D::dotProduct(direction.normalized(), normal.normalized()) / direction.lengthSquared() * normal.length();
    assert(!isnan(c));
    if(c < 0) c = 0;
    return c * intensity.mul(part);
  }
}

Ray AreaLight::getRandomRay(const Sample &sample1, const Sample &sample2, float &pdf) const
{
  QVector3D location = getLocation(sample1.getSample());
  QVector3D direction = sample2.getCosineWeightedDirection(normal, pdf);
  return Ray(location, direction);
}

QVector3D AreaLight::getLocation(QPointF p) const
{
  return origin + p.x() * uDirection + p.y() * vDirection;
}
