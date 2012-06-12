#ifndef TONEMAPPER_H
#define TONEMAPPER_H

#include <QImage>

#include <opencv2/core/core.hpp>

#include "global.h"

class Tonemapper
{
public:
  Tonemapper(QSize size): image(size, QImage::Format_RGB32)
  {
    
  }
  
  virtual QImage tonemap(const cv::Mat & film);
  
protected:
  virtual QRgb map(const cv::Mat & film, int row, int column);
  int clamp(float x);
  
private:
  QImage image;
  
};

#endif // TONEMAPPER_H
