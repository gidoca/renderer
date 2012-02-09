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
