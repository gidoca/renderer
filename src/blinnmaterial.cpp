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
#include "blinnmaterial.h"

#include "hitrecord.h"
#include "vechelper.h"
#include "sampler.h"

#include <cmath>
#include <algorithm>

BlinnMaterial::BlinnMaterial(cv::Vec3f kd, cv::Vec3f ks, float exponent) : kd(kd), ks(ks), exponent(exponent)
{
}

float G(const QVector3D& wi, const QVector3D& wo, const QVector3D& normal)
{
    using std::min;
    QVector3D wh = wi + wo;
    wh.normalize();
    return min(1.f, 2 * QVector3D::dotProduct(normal, wh) * min(QVector3D::dotProduct(normal, wo), QVector3D::dotProduct(normal, wi)) / QVector3D::dotProduct(wo, wh));
}

float BlinnMaterial::D(const QVector3D& wh, const QVector3D& normal) const
{
    return (exponent + 2) / (2 * M_PI) * pow(QVector3D::dotProduct(wh, normal), exponent);
}

cv::Vec3f BlinnMaterial::brdf(const HitRecord &hit, QVector3D direction) const
{
    QVector3D wo = -hit.getRay().getDirection();
    QVector3D wh = direction + wo;
    wh.normalize();
    return kd * (1 / M_PI) + ks * D(wh, hit.getSurfaceNormal()) * G(direction, wo, hit.getSurfaceNormal()) / (4 * QVector3D::dotProduct(wo, hit.getSurfaceNormal()) * QVector3D::dotProduct(direction, hit.getSurfaceNormal()));
}

QVector3D BlinnMaterial::outDirection(const HitRecord &hit, Sample s, float &pdf, cv::Vec3f &brdf) const
{
    QVector3D wo = -hit.getRay().getDirection();
    QVector3D wh = s.getCosinePowerWeightedDirection(hit.getSurfaceNormal(), pdf, exponent);
    if(QVector3D::dotProduct(wh, hit.getSurfaceNormal()) < 0) wh *= -1;
    QVector3D out = reflect(wo, wh);
    if(QVector3D::dotProduct(wo, wh) <= 0)
    {
        pdf = 0;
    }
    else
    {
        pdf /= 4 * QVector3D::dotProduct(wo, wh);
    }
    brdf = this->brdf(hit, out);
    return out;
}
