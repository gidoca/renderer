#ifndef FILM_H
#define FILM_H

#include <cstdlib>
#include <string>

#include <QSize>

#include <ImfRgba.h>

#include "global.h"
#include "spectrum.h"

class Film
{
public:
  Film(QSize size): data(new Spectrum[size.width() * size.height()]), size(size)
  {
  }

  Film(): data(0) {}

  inline void setSize(QSize size)
  {
    data = new Spectrum[size.width() * size.height()];
    this->size = size;
  }
  
  const inline Spectrum * operator[](int i) const
  {
    return data + i * size.width();
  }
  
  inline Spectrum * operator[](int i)
  {
    return data + i * size.width();
  }
  
  inline int width()
  {
    return size.width();
  }
  
  inline int height()
  {
    return size.height();
  }

  inline QSize getSize() const
  {
    return size;
  }

  void saveImg(std::string filename);
  void saveExr(std::string filename);

private:
  Imf::Rgba* toExrRgba() const;
  
  Spectrum* data;
  QSize size;
};

#endif // FILM_H
