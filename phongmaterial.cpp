#include "phongmaterial.h"
#include "hitrecord.h"
#include "light.h"

#include <cmath>

Spectrum PhongMaterial::shade(HitRecord& hit, QVector3D direction) const
{
  QVector3D normal = hit.getSurfaceNormal();
  double coefficient = QVector3D::dotProduct(-direction.normalized(), hit.getSurfaceNormal().normalized());
  coefficient = coefficient > 0 ? coefficient : 0;
  QVector3D reflected = 2 * QVector3D::dotProduct(-direction, normal) * normal + direction;
  double spec = pow(QVector3D::dotProduct(reflected.normalized(), hit.getRay().getDirection().toVector3D().normalized()), specularCoefficient);
  return coefficient * color + spec * specularColor;// + Spectrum(.04, .04, .04);
}
