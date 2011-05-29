#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"
#include "spectrum.h"

#include <QVector3D>

class PointLight:public Light
{

public:
  PointLight(QVector3D location, Spectrum intensity) : location(location), intensity(intensity)
  {

  }


  Spectrum getIntensity (HitRecord & hit, QVector3D & direction, const Intersectable & scene, Sample sample) const;
  QVector3D getDirection (QVector3D at) const;
  bool isOccluded(QVector3D location, const Intersectable & scene) const;
  
private:
  QVector3D location;
  Spectrum intensity;
};

#endif // POINTLIGHT_H
