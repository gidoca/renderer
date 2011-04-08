#include "phongmaterial.h"
#include "hitrecord.h"
#include "light.h"

#include <cmath>
#include <algorithm>

Spectrum PhongMaterial::shade(HitRecord& hit, QVector3D direction) const
{
  QVector3D normal = hit.getSurfaceNormal();
  double coefficient = std::max(QVector3D::dotProduct(-direction.normalized(), hit.getSurfaceNormal().normalized()), 0.0);
  QVector3D reflected = 2 * QVector3D::dotProduct(-direction, normal) * normal + direction;
  double spec = pow(QVector3D::dotProduct(reflected.normalized(), hit.getRay().getDirection().toVector3D().normalized()), specularCoefficient);
  return coefficient * color + spec * specularColor;
}
