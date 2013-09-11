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
#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "global.h"
#include "light.h"
#include "jitteredsampler.h"

#include <QtGui/QVector3D>
#include <QtCore/QPointF>

class AreaLight : public Light
{
public:
    AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, cv::Vec3f intensity);

    cv::Vec3f getIntensity(const HitRecord & hit, QVector3D &direction, const Intersectable & scene, const Sample &sample) const;
    Ray getRandomRay(const Sample &sample1, const Sample &sample2, float &pdf) const;

    HitRecord intersect(Ray ray) const;
    AxisAlignedBox* createBoundingBox();

private:
    QVector3D getLocation(QPointF p) const;

    QVector3D origin;
    QVector3D uDirection;
    QVector3D vDirection;
    cv::Vec3f intensity;
    QVector3D normal;
};

#endif // AREALIGHT_H
