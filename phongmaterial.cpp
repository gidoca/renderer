#include "phongmaterial.h"
#include "hitrecord.h"
#include "light.h"

#include <cmath>

PhongMaterial::PhongMaterial(Spectrum color, Spectrum specularColor, double specularCoefficient) : color(color), specularColor(specularColor), specularCoefficient(specularCoefficient)
{

}

Spectrum PhongMaterial::shade(HitRecord& hit, const Light& light) const
{
  Spectrum lightIntensity = light.getIntensity(hit.getIntersectingPoint());
  QVector3D direction = -light.getDirection(hit.getIntersectingPoint());
  QVector3D normal = hit.getSurfaceNormal();
  double coefficient = QVector3D::dotProduct(direction.normalized(), hit.getSurfaceNormal().normalized());
  lightIntensity = coefficient > 0 ? coefficient * lightIntensity : Spectrum();
  QVector3D reflected = 2 * QVector3D::dotProduct(direction, normal) * normal - direction;
  Spectrum diffuseSpectrum(lightIntensity.x() * color.x(), lightIntensity.y() * color.y(), lightIntensity.z() * color.z());
  Spectrum specularSpectrum(lightIntensity.x() * specularColor.x(), lightIntensity.y() * specularColor.y(), lightIntensity.z() * specularColor.z());
  specularSpectrum *= pow(QVector3D::dotProduct(reflected.normalized(), hit.getRay().getDirection().toVector3D().normalized()), specularCoefficient);
  return diffuseSpectrum + specularSpectrum;// + Spectrum(.04, .04, .04);
}
