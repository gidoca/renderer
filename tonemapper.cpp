#include "tonemapper.h"

#include "spectrum.h"
#include "film.h"

#define clamp(x) ((x) <= 0 ? 0 : ((x) >= 255 ? 255 : (x)))

QImage Tonemapper::tonemap(const Film& film)
{
  for(int i = 0; i < image.height(); i++)
  {
    QRgb * scanline = (QRgb*) image.scanLine(i);
    for(int j = 0; j < image.width(); j++)
    {
      scanline[j] = map(film[i][j]);
    }
  }
  return image;
}

QRgb Tonemapper::map(const Spectrum& s)
{
  Spectrum irradiance = 255 * s;
  return qRgb((int) clamp(irradiance.x()), (int) clamp(irradiance.y()), (int) clamp(irradiance.z()));

}

