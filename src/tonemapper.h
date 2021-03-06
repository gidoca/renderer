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
#ifndef TONEMAPPER_H
#define TONEMAPPER_H

#include <QImage>

#include <opencv2/core/core.hpp>

#include "global.h"

class Tonemapper
{
public:
  Tonemapper(QSize size): image(size, QImage::Format_RGB32), gamma(2.2)
  {
  }

  Tonemapper(QSize size, float gamma) : image(size, QImage::Format_RGB32), gamma(gamma)
  {
  }
  
  virtual QImage tonemap(const cv::Mat film);
  
protected:
  virtual QRgb map(const cv::Mat & film, int row, int column);
  int clamp(float x);
  
private:
  QImage image;
  float gamma;
  
};

#endif // TONEMAPPER_H
