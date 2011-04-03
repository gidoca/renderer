#include <QSize>
#include <QImage>
#include <QSharedPointer>
#include <list>
#include <iostream>

#include "tmscene.h"

#define clamp(x) ((x) <= 0 ? 0 : ((x) >= 255 ? 255 : (x)))

int main(int argc, char **argv) {
  QSize resolution(512, 512);

  QImage image(resolution, QImage::Format_RGB32);
  
  const Intersectable * object = getScene();
  const std::list<QSharedPointer<Light> > light = getLight();
  const Camera camera = getCamera(resolution);
  
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < image.height(); i++)
  {
    QRgb * scanline = (QRgb *) image.scanLine(i);
    for(int j = 0; j < image.width(); j++)
    {
      if(i == 200 && j == 300)
      {
        std::cout << "break" << std::endl;
      }
      QPoint point = QPoint(j, i);
      Ray ray = camera.getRay(point);
      HitRecord hitRecord = object->intersect(ray);
      Spectrum irradiance;
      for(std::list<QSharedPointer<Light> >::const_iterator i = light.begin(); i != light.end(); i++)
      {
        irradiance += 255 * hitRecord.getMaterial().shade(hitRecord, **i, *object, 0);
      }
  
      scanline[j] = qRgb((int) clamp(irradiance.x()), (int) clamp(irradiance.y()), (int) clamp(irradiance.z()));
    }
    std::cout << i * 100 / image.height() << "% complete" << std::endl;
  }
  
  delete object;
  
  image.save("tst.png");
  return 0;
}
