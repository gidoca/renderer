#include "perpixelrenderer.h"

#include "intersectable.h"
#include "camera.h"
#include "light.h"
#include "unidipathtracingintegrator.h"
#include "jitteredsampler.h"

#include <iostream>

#include <QTime>

#define clamp(x) ((x) <= 0 ? 0 : ((x) >= 255 ? 255 : (x)))

PerPixelRenderer::PerPixelRenderer(QSize resolution): image(resolution, QImage::Format_RGB32)
{

}

QImage PerPixelRenderer::render(const Intersectable& scene, const Camera& camera, std::list< QSharedPointer< Light > > lights)
{
  QTime time;
  time.start();
	
  Integrator * integrator = new UniDiPathTracingIntegrator();
//  Integrator * integrator = new SimpleIntegrator();
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < image.height(); i++)
  {
    qsrand(i);
    std::cout << i * 100 / image.height() << "% complete, ETA: " << time.elapsed() * (image.height() - i) / ((i + 1) * 1000) << "s" << std::endl;
    QRgb * scanline = (QRgb *) image.scanLine(i);
    for(int j = 0; j < image.width(); j++)
    {
#ifndef NDEBUG
      if(j == 70 && i == 120)
        std::cout << "";
#endif
      JitteredSampler multiSampler(1, 1);
      std::list<Sample> samples = multiSampler.getSamples();
      QPointF point = QPoint(j, i);
      Spectrum irradiance;
      for(std::list<Sample>::iterator it = samples.begin(); it != samples.end(); it++)
      {
        QPointF samplePoint = point + it->getSample() - QPointF(0.5, 0.5);
        Ray ray = camera.getRay(samplePoint);
        irradiance += 255 * integrator->integrate(ray, scene, lights) / samples.size();
      }
  
      scanline[j] = qRgb((int) clamp(irradiance.x()), (int) clamp(irradiance.y()), (int) clamp(irradiance.z()));
    }
  }

  std::cout << "100% complete, time elapsed: " << time.elapsed() / 1000 << "s\n";
  std::cout.flush();
	
	return image;
}


