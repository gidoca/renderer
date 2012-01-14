#ifndef TONEMAPPER_H
#define TONEMAPPER_H

#include <QImage>

class Film;
class Spectrum;

class Tonemapper
{
public:
  Tonemapper(QSize size): image(size, QImage::Format_RGB32)
  {
    
  }
  
  QImage tonemap(const Film & film);
  
protected:
  QRgb map(const Spectrum & s);
  
private:
  QImage image;
  
};

#endif // TONEMAPPER_H
