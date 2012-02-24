#include "film.h"

Imf::Rgba* Film::toExrRgba() const
{
  Imf::Rgba *result = new Imf::Rgba[size.width() * size.height()];
  for(int i = 0; i < size.width(); i++)
  {
    for(int j = 0; j < size.height(); j++)
    {
      Spectrum s = (*this)[j][i];
      result[i + size.width() * j] = Imf::Rgba(s.x(), s.y(), s.z(), 1.f);
    }
  }
  return result;
}
