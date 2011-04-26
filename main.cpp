#include <QtCore/QSize>
#include <QtGui/QImage>
#include <QtCore/QSharedPointer>
#include <QtCore/QTime>

#include <list>
#include <iostream>

#include "cornellscene.h"
#include "pathtracingintegrator.h"
#include "simpleintegrator.h"
#include "jitteredsampler.h"

#define clamp(x) ((x) <= 0 ? 0 : ((x) >= 255 ? 255 : (x)))

int main(int, char **) {
  QTime time;
  time.start();
  QSize resolution(256, 256);

  QImage image(resolution, QImage::Format_RGB32);

  Integrator * integrator = new PathTracingIntegrator();
//  Integrator * integrator = new SimpleIntegrator();
  
  const Intersectable * object = getScene();
  const std::list<QSharedPointer<Light> > light = getLight();
  const Camera camera = getCamera(resolution);
  
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < image.height(); i++)
  {
    std::cout << i * 100 / image.height() << "% complete, ETA: " << time.elapsed() * (image.height() - i) / ((i + 1) * 1000) << "s" << std::endl;
    JitteredSampler multiSampler(8, 8);
    std::list<Sample> samples = multiSampler.getSamples();
    QRgb * scanline = (QRgb *) image.scanLine(i);
    for(int j = 0; j < image.width(); j++)
    {
      if(j == 130 && i == 140)
        std::cout << std::endl;
      QPointF point = QPoint(j, i);
      Spectrum irradiance;
      for(std::list<Sample>::iterator i = samples.begin(); i != samples.end(); i++)
      {
        QPointF samplePoint = point + i->getSample() - QPointF(0.5, 0.5);
        Ray ray = camera.getRay(samplePoint);
        irradiance += 255 * integrator->integrate(ray, *object, light) / samples.size();
      }
  
      scanline[j] = qRgb((int) clamp(irradiance.x()), (int) clamp(irradiance.y()), (int) clamp(irradiance.z()));
    }
  }
  
  delete object;
  
  if(!image.save("/tmp/tst.png")) std::cout << "Failed to save file.\n";
  std::cout << "100% complete, time elapsed: " << time.elapsed() / 1000 << "s\n";
  return 0;
}
