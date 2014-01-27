/**
 * Copyright (C) 2012 Gian Calgeer
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "phongmaterial.h"
#include "hitrecord.h"
#include "light.h"
#include "sampler.h"
#include "vechelper.h"

#include <cmath>
#include <algorithm>
#include <cassert>

cv::Vec3f PhongMaterial::brdf(const HitRecord& hit, QVector3D direction) const
{
  return diffuseBrdf() + specularBrdf(hit, direction);
}

cv::Vec3f PhongMaterial::diffuseBrdf() const
{

  return color * (1 / M_PI);
}

cv::Vec3f PhongMaterial::specularBrdf(const HitRecord& hit, QVector3D direction) const
{
  QVector3D normal = hit.getSurfaceNormal().normalized();
  QVector3D reflected = reflect(direction, normal);
  float dp = std::max(QVector3D::dotProduct(reflected.normalized(), -hit.getRay().getDirection().normalized()), 0.f);
  float spec = (specularCoefficient + 2) / (2 * M_PI) * pow(dp, specularCoefficient);
  return spec * specularColor;
}

QVector3D PhongMaterial::outDirection(const HitRecord &hit, Sample s, float &pdf, cv::Vec3f &brdf) const
{
    QVector3D surfaceNormal = hit.getSurfaceNormal();
    surfaceNormal.normalize();
    QVector3D specularDirection = reflect(hit.getRay().getDirection(), surfaceNormal);
    float rho_d = lum(color);
    float rho_s = lum(specularColor);
    assert(rho_d + rho_s <= 1);
    if(s.getSample().x() < rho_d)
    {
        s.getSample().rx() /= rho_d;
        QVector3D out = s.getCosineWeightedDirection(surfaceNormal, pdf);
        brdf = diffuseBrdf();
        return out;
    }
    else if(s.getSample().x() < rho_d + rho_s)
    {
        s.getSample().rx() = (s.getSample().x() - rho_d) / rho_s;
        QVector3D out = s.getCosinePowerWeightedDirection(specularDirection, pdf, specularCoefficient);
        if(QVector3D::dotProduct(out, surfaceNormal) < 1) pdf = 0;
        brdf = specularBrdf(hit, out);
        return out;
    }
    else
    {
        pdf = 0;
        brdf = cv::Vec3f();
        return QVector3D();
    }
}
