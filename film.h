#ifndef FILM_H
#define FILM_H

#include <cstdlib>

#include <QSize>

#include "spectrum.h"

class Film
{
public:
  Film(QSize size): size(size)
  {
    data = (Spectrum *) calloc(size.width() * size.height(), sizeof(Spectrum));
  }
  
  ~Film()
  {
    free(data);
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
  
private:
  Spectrum * data;
  QSize size;
};

#endif // FILM_H
