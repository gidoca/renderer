#include "phongmaterial.h"
#include "hitrecord.h"
#include "light.h"

#include <cmath>
#include <algorithm>
#include <cassert>

Spectrum PhongMaterial::shade(const HitRecord& hit, QVector3D direction) const
{
//   QVector3D normal = hit.getSurfaceNormal();
//   float coefficient = std::max(QVector3D::dotProduct(-direction.normalized(), hit.getSurfaceNormal().normalized()), 0.0);
//   assert(coefficient >= 0);
//   QVector3D reflected = 2 * QVector3D::dotProduct(-direction, normal) * normal + direction;
//   float spec = pow(QVector3D::dotProduct(reflected.normalized(), hit.getRay().getDirection().normalized()), specularCoefficient);
  return /* coefficient * */ color / M_PI; //+ spec * specularColor;
}
