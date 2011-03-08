#include "diffusematerial.h"
#include "hitrecord.h"

DiffuseMaterial::DiffuseMaterial(Spectrum color) : color(color)
{

}

Spectrum DiffuseMaterial::shade(HitRecord & hit, Light & light) const
{
  Spectrum lightIntensity = light.getIntensity(hit.getIntersectingPoint());
  QVector3D direction = light.getDirection(hit.getIntersectingPoint());
  double coefficient = QVector3D::dotProduct(direction.normalized(), hit.getSurfaceNormal().normalized());
  lightIntensity = coefficient > 0 ? coefficient * lightIntensity : Spectrum();
  return Spectrum(lightIntensity.x() * color.x(), lightIntensity.y() * color.y(), lightIntensity.z() * color.z());
}
