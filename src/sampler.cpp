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
  w.normalize();
  QVector3D normalizedDirection = QVector3D(cos(2 * M_PI * sample.y()) * sqrt(sample.x()), sin(2 * M_PI * sample.y()) * sqrt(sample.x()), sqrt(1 - sample.x()));
//  std::cout << sample.x() << std::endl;
  pdf = normalizedDirection.z() / M_PI;
//  std::cout << normalizedDirection.z() << std::endl;
//  normalizedDirection.normalize();
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

QVector3D Sample::getCosineWeightedDirection(QVector3D w, float &pdf, float openingAngle) const
{
  float radiusFactor = sin(openingAngle);
  w.normalize();
  QVector3D normalizedDirection = QVector3D(cos(2 * M_PI * sample.y()) * sqrt(sample.x()) * radiusFactor, sin(2 * M_PI * sample.y()) * sqrt(sample.x()) * radiusFactor, sqrt(1 - sample.x() * radiusFactor * radiusFactor));
//  std::cout << sample.x() << std::endl;
  pdf = normalizedDirection.z() / M_PI;
//  std::cout << normalizedDirection.z() << std::endl;
//  normalizedDirection.normalize();
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
  return (float) qrand() / RAND_MAX;
}
