#include "directionallight.h"

#include <iostream>

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

bool DirectionalLight::isOccluded(QVector3D location, const Intersectable & scene) const
{
  HitRecord hit = scene.intersect(Ray(location, -direction.normalized()));
  return hit.intersects();
}
