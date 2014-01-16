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
#include "sampler.h"

#include <cstdlib>
#include <cmath>
#include <assert.h>

#include <QtCore/QtGlobal>
#include <QtCore/QTime>
#include <QtGui/QMatrix4x4>

#include <iostream>

QVector3D Sample::getCosineWeightedDirection(QVector3D w, float & pdf) const
{
    return getCosinePowerWeightedDirection(w, pdf, 1);
}

QVector3D Sample::getCosineWeightedDirection(QVector3D w, float &pdf, float openingAngle) const
{
    return getCosinePowerWeightedDirection(w, pdf, 1, openingAngle);
}

QVector3D Sample::getCosinePowerWeightedDirection(QVector3D w, float &pdf, float cosineExponent) const
{
    return getCosinePowerWeightedDirection(w, pdf, cosineExponent, M_PI / 2);
}

QVector3D Sample::getCosinePowerWeightedDirection(QVector3D w, float &pdf, float cosineExponent, float openingAngle) const
{
  float radiusFactor = sin(openingAngle);
  w.normalize();
  QVector3D normalizedDirection = QVector3D(cos(2 * M_PI * sample.y()) * sqrt(1 - pow(sample.x(), 2 / (cosineExponent + 1)) * radiusFactor * radiusFactor), sin(2 * M_PI * sample.y()) * sqrt(1 - pow(sample.x(), 2 /(cosineExponent + 1)) * radiusFactor * radiusFactor), pow(sample.x(), 1 / (cosineExponent + 1)) * radiusFactor);
  pdf =  pow(normalizedDirection.z(), cosineExponent) * (cosineExponent + 1) / (2 * M_PI);
  QVector3D v;
  if(abs(w.x()) < abs(w.y()) && abs(w.x()) < abs(w.z()))
  {
    v = QVector3D(0, w.z(), -w.y());
  }
  else if(abs(w.y()) < abs(w.z()))
  {
    v = QVector3D(w.z(), 0, -w.x());
  }
  else
  {
    v = QVector3D(w.y(), -w.x(), 0);
  }
  v.normalize();
  QVector3D u = QVector3D::crossProduct(v, w);
  QMatrix4x4 transform;
  transform.setColumn(0, u);
  transform.setColumn(1, v);
  transform.setColumn(2, w);
  QVector3D randomDirection = transform.map(normalizedDirection);
  assert(QVector3D::dotProduct(randomDirection, w) >= 0);
  return randomDirection;
}

QVector3D Sample::getUniformSphereDirection() const
{
  float z = 2 * sample.x() - 1;
  float t = 2 * M_PI * sample.y();
  float f = sqrt(1 - z * z);
  float x = f * cos(t);
  float y = f * sin(t);
  return QVector3D(x, y, z);
}

float Sampler::getRandom()
{
  return gsl_rng_uniform(rng);
}
