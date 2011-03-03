#include "directionallight.h"

DirectionalLight::DirectionalLight(QVector3D direction, Spectrum intensity) : direction(direction), intensity(intensity)
{

}

Spectrum DirectionalLight::getIntensity(Ray ray) const
{
  return QVector3D::dotProduct(direction, ray.getDirection().toVector3D()) * intensity;
}
