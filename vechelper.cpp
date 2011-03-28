#include "vechelper.h"

QVector3D operator*(const QVector3D & v1, const QVector3D & v2)
{
  return QVector3D(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

QVector3D operator/(const QVector3D & v1, const QVector3D & v2)
{
  return QVector3D(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
}