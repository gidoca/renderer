#include "tonemapper.h"

#include "spectrum.h"
#include "film.h"

inline int Tonemapper::clamp(float x)
{
  return x <= 0 ? 0 : (x >= 255 ? 255 : x);
}

QImage Tonemapper::tonemap(const Film& film)
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

QRgb Tonemapper::map(const Film & film, int row, int column)
{
  Spectrum irradiance = 255 * film[row][column];
  return qRgb(clamp(irradiance.x()), clamp(irradiance.y()), clamp(irradiance.z()));
}

