#ifndef DIRECTIONALLIGHT_H
#define DRIECTINOALLIGHT_H

#include "spectrum.h"
#include "light.h"

#include <QVector3D>

class DirectionalLight : public Light
{
  public:
    DirectionalLight(QVector3D direction, Spectrum intensity) : direction(direction), intensity(intensity)
    {

    }


    Spectrum getDirection() const;
    Spectrum getIntensity(HitRecord &, QVector3D & direction, const Intersectable & scene, QPointF) const;
    bool isOccluded(QVector3D location, const Intersectable & scene) const;

  private:
    QVector3D direction;
    Spectrum intensity;
};

#endif
