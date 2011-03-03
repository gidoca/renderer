#ifndef MATERIAL_H
#define MATERIAL_H

#include "spectrum.h"

#include <QVector3D>

class Material
{
  public:
    virtual Spectrum shade(QVector3D location, QVector3D normal) const = 0;
};

#endif
