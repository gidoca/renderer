#ifndef TONEMAPPER_H
#define TONEMAPPER_H

#include <QImage>

class Film;

class Tonemapper
{
public:
  Tonemapper(QSize size): image(size, QImage::Format_RGB32)
  {
    
  }
  
  virtual QImage tonemap(const Film & film);
  
protected:
  virtual QRgb map(const Film & film, int row, int column);
  int clamp(float x);
  
private:
  QImage image;
  
};

#endif // TONEMAPPER_H
