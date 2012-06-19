#include "tonemapper.h"

#include <cmath>

#include <opencv2/core/core.hpp>

using namespace cv;

inline int Tonemapper::clamp(float x)
{
  return x <= 0 ? 0 : (x >= 255 ? 255 : x);
}

QImage Tonemapper::tonemap(const cv::Mat &film)
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
  //OpenCV uses BGR byte order
  return qRgb(clamp(255 * pow(irradiance[2], 1./2.3)), clamp(255 * pow(irradiance[1], 1./2.3)), clamp(255 * pow(irradiance[0], 1./2.3)));
}

