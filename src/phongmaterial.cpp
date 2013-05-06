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

#include <cmath>
#include <algorithm>
#include <cassert>

cv::Vec3f PhongMaterial::shade(const HitRecord& hit, QVector3D direction) const
{
  QVector3D normal = hit.getSurfaceNormal().normalized();
  QVector3D reflected = 2 * QVector3D::dotProduct(-direction, normal) * normal + direction;
  float dp = std::max(QVector3D::dotProduct(reflected.normalized(), -hit.getRay().getDirection().normalized()), 0.f);
  float spec = pow(dp, specularCoefficient);
  return color * (1 / M_PI) + spec * specularColor;
}
