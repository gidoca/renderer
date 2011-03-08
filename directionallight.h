#ifndef DIRECTIONALLIGHT_H
#define DRIECTINOALLIGHT_H

#include "light.h"

class DirectionalLight : public Light
{
  public:
    DirectionalLight(QVector3D direction, Spectrum intensity);

    Spectrum getDirection(QVector3D) const;
    Spectrum getIntensity(QVector3D) const;

  private:
    QVector3D direction;
    Spectrum intensity;
};

#endif
