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
#ifndef SAMPLER_H
#define SAMPLER_H

#include <list>

#include <QtCore/QPointF>
#include <QtGui/QVector3D>

#include <gsl/gsl_rng.h>

#include "global.h"

class Sample
{
public:
  Sample(QPointF sample) : sample(sample) {}
  Sample() {}

  const QPointF& getSample() const;
  QPointF& getSample();
  QVector3D getCosineWeightedDirection(QVector3D w, float & pdf) const;
  QVector3D getCosineWeightedDirection(QVector3D w, float &pdf, float openingAngle) const;
  QVector3D getUniformSphereDirection() const;

private:
  QPointF sample;
};

class Sampler
{
public:
  Sampler(gsl_rng *rng) : rng(rng)
  {
  }
  virtual std::list<Sample> getSamples() = 0;

protected:
  float getRandom();
    
  gsl_rng * rng;
};

inline QPointF& Sample::getSample()
{
  return sample;
}

inline const QPointF& Sample::getSample() const
{
  return sample;
}

#endif // SAMPLER_H
