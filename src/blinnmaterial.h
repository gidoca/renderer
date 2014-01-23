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
#ifndef BLINNMATERIAL_H
#define BLINNMATERIAL_H

#include "material.h"

#include <opencv2/core/core.hpp>

#include <QVector3D>

class BlinnMaterial : public Material
{
public:
    BlinnMaterial(cv::Vec3f kd, cv::Vec3f ks, float exponent);
    cv::Vec3f brdf(const HitRecord &hit, QVector3D direction) const;
    QVector3D outDirection(const HitRecord & hit, Sample s, float& pdf, cv::Vec3f& brdf) const;

private:
    float D(const QVector3D& wh, const QVector3D& normal) const;

    cv::Vec3f kd, ks;
    float exponent;
};

#endif // BLINNMATERIAL_H
