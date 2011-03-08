#include "directionallight.h"

DirectionalLight::DirectionalLight(QVector3D direction, Spectrum intensity) : direction(direction), intensity(intensity)
{

}

Spectrum DirectionalLight::getDirection(QVector3D) const
{
  return direction;
}

Spectrum DirectionalLight::getIntensity(QVector3D) const
{
  return intensity;
}
