#include "scene2.h"

#include <QSize>
#include <QImage>
#include <list>
#include <iostream>

#define clamp(x) ((x) <= 0 ? 0 : ((x) >= 255 ? 255 : (x)))

int main(int argc, char **argv) {
  QSize resolution(256, 256);

  QImage image(resolution, QImage::Format_RGB32);
  
  const Intersectable * object = getScene();
  Light * light = getLight();
  Camera camera = getCamera(resolution);
  
  for(int i = 0; i < image.height(); i++)
  {
    QRgb * scanline = (QRgb *) image.scanLine(i);
    #pragma omp parallel for
    for(int j = 0; j < image.width(); j++)
    {
      QPoint point = QPoint(j, i);
      Ray ray = camera.getRay(point);
      HitRecord hitRecord = object->intersect(ray);
      Spectrum irradiance = 255 * hitRecord.getMaterial().shade(hitRecord, *light, *object, 0);
  
      scanline[j] = qRgb((int) clamp(irradiance.x()), (int) clamp(irradiance.y()), (int) clamp(irradiance.z()));
    }
    std::cout << i * 100 / image.height() << "% complete" << std::endl;
  }
  
  delete light;
  delete object;
  
  image.save("tst.png");
  return 0;
}
