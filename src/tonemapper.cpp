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
#include "tonemapper.h"

#include <cmath>

#include <opencv2/core/core.hpp>

using namespace cv;

inline int Tonemapper::clamp(float x)
{
  return x <= 0 ? 0 : (x >= 255 ? 255 : x);
}

QImage Tonemapper::tonemap(const cv::Mat film)
{
  for(int i = 0; i < image.height(); i++)
  {
    QRgb * scanline = (QRgb*) image.scanLine(i);
    for(int j = 0; j < image.width(); j++)
    {
      scanline[j] = map(film, i, j);
    }
  }
  return image;
}

QRgb Tonemapper::map(const cv::Mat &film, int row, int column)
{
  Vec3f irradiance = film.at<Vec3f>(row, column);
  assert(checkRange(irradiance));
  //OpenCV uses BGR byte order
  return qRgb(clamp(255 * pow(irradiance[2], 1./2.3)), clamp(255 * pow(irradiance[1], 1./2.3)), clamp(255 * pow(irradiance[0], 1./2.3)));
}

