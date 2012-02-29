#include "film.h"

#include "tonemapper.h"

#include <QImage>

#include <ImfRgbaFile.h>

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

void Film::saveImg(std::string filename)
{
  Tonemapper tonemapper(size);
  QImage image = tonemapper.tonemap(*this);
  image.save(QString::fromStdString(filename));
}

void Film::saveExr(std::string filename)
{
  Imf::Rgba *rgba = toExrRgba();
  Imf::RgbaOutputFile file(filename.c_str(), size.width(), size.height(), Imf::WRITE_RGBA);
  file.setFrameBuffer(rgba, 1, size.width());
  file.writePixels(size.height());
}
