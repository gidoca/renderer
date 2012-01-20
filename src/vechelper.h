#ifndef VECHELPER_H
#define VECHELPER_H

#include <QtGui/QVector3D>

inline float get(const QVector3D& v, int i)
{
  switch(i)
  {
    case 0: return v.x();
    case 1: return v.y();
    case 2: return v.z();
    default: return 0;
  }
}

#endif
