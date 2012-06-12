#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "global.h"
#include "light.h"
#include "spectrum.h"

#include <QVector3D>

class PointLight:public Light
{

public:
  PointLight(QVector3D location, cv::Vec3f intensity) : location(location), intensity(intensity)
  {

  }


  cv::Vec3f getIntensity (const QVector3D & at, QVector3D & direction, const Intersectable & scene, const Sample &sample) const;
  virtual Ray getRandomRay(const Sample& sample, const Sample&, float& pdf) const;
  QVector3D getDirection (QVector3D at) const;
  bool isOccluded(QVector3D location, const Intersectable & scene) const;
  
private:
  QVector3D location;
  cv::Vec3f intensity;
};

#endif // POINTLIGHT_H
