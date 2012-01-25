#ifndef FILM_H
#define FILM_H

#include <cstdlib>

#include <QSize>
#include <QSharedPointer>

#include "global.h"
#include "spectrum.h"

class Film
{
public:
  Film(QSize size): data(new Spectrum[size.width() * size.height()]), size(size)
  {
  }
  
  const inline Spectrum * operator[](int i) const
  {
    return data.data() + i * size.width();
  }
  
  inline Spectrum * operator[](int i)
  {
    return data.data() + i * size.width();
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
  
private:
  QSharedPointer<Spectrum> data;
  QSize size;
};

#endif // FILM_H
