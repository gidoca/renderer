#ifndef DIRECTIONALLIGHT_H
#define DRIECTINOALLIGHT_H

#include "light.h"

class DirectionalLight : public Light
{
  public:
    DirectionalLight(QVector3D direction, Spectrum intensity);

    Spectrum getIntensity(Ray ray) const;

  private:
    QVector3D direction;
    Spectrum intensity;
};

#endif
