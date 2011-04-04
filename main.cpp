#include <QSize>
#include <QImage>
#include <QSharedPointer>
#include <QTime>

#include <list>
#include <iostream>

#include "scene1.h"
#include "integrator.h"
#include "jitteredsampler.h"

#define clamp(x) ((x) <= 0 ? 0 : ((x) >= 255 ? 255 : (x)))

int main(int argc, char **argv) {
  QTime time;
  time.start();
  QSize resolution(512, 512);

  QImage image(resolution, QImage::Format_RGB32);

  Integrator integrator;
  
  const Intersectable * object = getScene();
  const std::list<QSharedPointer<Light> > light = getLight();
  const Camera camera = getCamera(resolution);
  
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < image.height(); i++)
  {
    JitteredSampler sampler(2, 2, i);
    std::list<QPointF> samples = sampler.getSamples();
    QRgb * scanline = (QRgb *) image.scanLine(i);
    for(int j = 0; j < image.width(); j++)
    {
      QPointF point = QPoint(j, i);
      Spectrum irradiance;
      for(std::list<QPointF>::iterator i = samples.begin(); i != samples.end(); i++)
      {
        QPointF samplePoint = point + *i;
        Ray ray = camera.getRay(samplePoint);
        irradiance += 255 * integrator.integrate(ray, *object, light, sampler) / samples.size();
      }
  
      scanline[j] = qRgb((int) clamp(irradiance.x()), (int) clamp(irradiance.y()), (int) clamp(irradiance.z()));
    }
    std::cout << i * 100 / image.height() << "% complete" << std::endl;
  }
  
  delete object;
  
  image.save("tst.png");
  std::cout << time.elapsed() << "ms\n";
  return 0;
}
