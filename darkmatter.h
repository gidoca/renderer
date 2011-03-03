#ifndef DARKMATTER_H
#define DARKMATTER_H

#include "material.h"

class DarkMatter : public Material
{
  public:
    Spectrum shade(QVector3D, QVector3D) const;
};

#endif
