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
#include "texturematerial.h"

#include "hitrecord.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <iostream>

using namespace cv;

inline cv::Vec3f getColorSubpix(const cv::Mat& img, cv::Point2f pt)
{
    cv::Mat patch;
    cv::getRectSubPix(img, cv::Size(1,1), pt, patch);
    return patch.at<cv::Vec3f>(0,0);
}

inline float clamp(float x)
{
//    float f = x <= 0 ? 0 : (x >= 1 ? 1 : x);
    float f = fmod(x, 1.d);
    if(f < 0) f += 1;
    assert(f >= 0 && f <= 1);
    return f;
}

bool TextureMaterial::load(std::string filename)
{
    Mat img = imread(filename);
    if(img.data == nullptr)
    {
        std::cerr << "Error loading texture \"" << filename << "\"" << std::endl;
        return false;
    }
    else
    {
        std::cerr << "Successfully loaded texture \"" << filename << "\"" << std::endl;
    }
    img.convertTo(texture, CV_32F, 1./255.);
    return true;
}

cv::Vec3f TextureMaterial::shade(const HitRecord & hit, QVector3D) const
{
    return get(hit.getTexCoords()) * (1. / M_PI);
}

cv::Vec3f TextureMaterial::get(Point2f location) const
{
    return getColorSubpix(texture, Point2f(clamp(location.x) * (texture.size().width - 1), clamp(1 - location.y) * (texture.size().height - 1))).mul(coefficient);
}
