#include "perpixelrenderer.h"

#include "intersectable.h"
#include "camera.h"
#include "light.h"
#include "unidipathtracingintegrator.h"
#include "jitteredsampler.h"
#include "integrator.h"

#include <iostream>

#include <QTime>
#include <QRgb>


PerPixelRenderer::PerPixelRenderer(QSize resolution, Integrator * integrator): integrator(integrator), resolution(resolution)
{

}

PerPixelRenderer::~PerPixelRenderer()
{
  delete integrator;
}

void PerPixelRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< Light* > lights, Film & film)
{
  QTime time;
  time.start();
  
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < film.height(); i++)
  {
    qsrand(i);
    std::cout << i * 100 / film.height() << "% complete, ETA: " << time.elapsed() * (film.height() - i) / ((i + 1) * 1000) << "s" << std::endl;
    Spectrum * scanline = film[i];
    for(int j = 0; j < film.width(); j++)
    {
#ifndef NDEBUG
      if(j == 70 && i == 120)
        std::cout << "";
#endif
      JitteredSampler multiSampler(1, 1);
      std::list<Sample> samples = multiSampler.getSamples();
      QPointF point = QPoint(j, i);
      for(std::list<Sample>::iterator it = samples.begin(); it != samples.end(); it++)
      {
        QPointF samplePoint = point + it->getSample() - QPointF(0.5, 0.5);
        Ray ray = camera.getRay(samplePoint);
        scanline[j] += integrator->integrate(ray, scene, lights) / samples.size();
      }
    }
  }

  std::cout << "100% complete, time elapsed: " << time.elapsed() / 1000 << "s\n";
  std::cout.flush();
}


