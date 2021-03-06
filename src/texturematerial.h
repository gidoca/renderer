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
#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "global.h"
#include "abstractdiffusematerial.h"

#include <string>

#include <opencv2/core/core.hpp>

#include <QVector3D>

class TextureMaterial : public AbstractDiffuseMaterial
{
public:
    TextureMaterial(cv::Vec3f coefficient = cv::Vec3f(1, 1, 1), float gamma = 1) : coefficient(coefficient), gamma(gamma) {}
    virtual ~TextureMaterial() {}

    bool load(std::string filename);
    cv::Vec3f brdf(const HitRecord & hit, QVector3D direction) const;

private:
    cv::Vec3f get(cv::Point2f location) const;

    cv::Mat texture;
    cv::Vec3f coefficient;
    float gamma;
};

#endif // TEXTUREMATERIAL_H
