#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"

class PointLight:public Light
{

public:
  PointLight(QVector3D location, Spectrum intensity);
  
  Spectrum getIntensity (QVector3D at) const;
  QVector3D getDirection (QVector3D at) const;
  bool isOccluded(QVector3D location, const Intersectable & scene) const;
  
private:
  QVector3D location;
  Spectrum intensity;
};

#endif // POINTLIGHT_H
