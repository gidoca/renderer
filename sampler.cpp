#include "sampler.h"

#include <cstdlib>
#include <cmath>
#include <assert.h>

#include <QtGlobal>
#include <QTime>
#include <QMatrix4x4>

#include <iostream>

QVector3D Sample::getCosineWeightedDirection(QVector3D w, double & pdf) const
{
  QVector3D normalizedDirection = QVector3D(cos(2 * M_PI * sample.y()) * sqrt(sample.x()), sin(2 * M_PI * sample.y()) * sqrt(sample.x()), sqrt(1 - sample.x()));
  pdf = normalizedDirection.z() / M_PI;
  normalizedDirection.normalize();
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

Sampler::Sampler(unsigned int seed)
{
  qsrand(seed);
}

double Sampler::getRandom()
{
  return (double) qrand() / RAND_MAX;
}
