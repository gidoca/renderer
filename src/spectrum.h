#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QtGui/QVector3D>

#include "global.h"

class Spectrum : public QVector3D
{
public:
  Spectrum() {}
  Spectrum(const QVector3D & other) : QVector3D(other) {}
  Spectrum(float x, float y, float z) : QVector3D(x, y, z) {}

  Spectrum operator*(const Spectrum & other);
};

inline Spectrum Spectrum::operator *(const Spectrum & other)
{
  return Spectrum(x() * other.x(), y() * other.y(), z() * other.z());
}

#endif
