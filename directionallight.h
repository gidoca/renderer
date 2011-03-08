#ifndef DIRECTIONALLIGHT_H
#define DRIECTINOALLIGHT_H

#include "light.h"

class DirectionalLight : public Light
{
  public:
    DirectionalLight(QVector3D direction, Spectrum intensity);

    Spectrum getDirection(QVector3D) const;
    Spectrum getIntensity(QVector3D) const;
    bool isOccluded(QVector3D location, const Intersectable & scene) const;

  private:
    QVector3D direction;
    Spectrum intensity;
};

#endif
