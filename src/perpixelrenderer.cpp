#include "perpixelrenderer.h"

#include "intersectable.h"
#include "camera.h"
#include "light.h"
#include "unidipathtracingintegrator.h"
#include "jitteredsampler.h"
#include "integrator.h"

#include <iostream>
#include <cassert>
#include <cmath>

#include <QTime>
#include <QRgb>

#include <gsl/gsl_rng.h>

using namespace boost::program_options;

PerPixelRenderer::PerPixelRenderer(Integrator * integrator): integrator(integrator)
{

}

PerPixelRenderer::~PerPixelRenderer()
{
  delete integrator;
}

void PerPixelRenderer::render(const Intersectable& scene, const Camera& camera, std::vector< Light* > lights, Film & film, boost::program_options::variables_map vm)
{
  QTime time;
  time.start();
  
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < film.height(); i++)
  {
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(rng, i);
    std::cout << i * 100 / film.height() << "% complete, ETA: " << time.elapsed() * (film.height() - i) / ((i + 1) * 1000) << "s" << std::endl;
    Spectrum * scanline = film[i];
    for(int j = 0; j < film.width(); j++)
    {
#ifndef NDEBUG
      //Black pixel
      if(j == 84 && i == 13)
        std::cout << "";
#endif
      JitteredSampler multiSampler(vm["ppr-x-samples"].as<int>(), vm["ppr-y-samples"].as<int>(), rng);
      std::list<Sample> samples = multiSampler.getSamples();
      QPointF point = QPoint(j, i);
      for(std::list<Sample>::iterator it = samples.begin(); it != samples.end(); it++)
      {
        QPointF samplePoint = point + it->getSample()/* - QPointF(0.5, 0.5)*/;
        Ray ray = camera.getRay(samplePoint);
        Spectrum s = integrator->integrate(ray, scene, lights, rng) / samples.size();
        assert(!isnan(s.x()) && !isnan(s.y()) && !isnan(s.z()));
        assert(s.x() >= 0 && s.y() >= 0 && s.z() >= 0);
        scanline[j] += s;
      }
    }
    gsl_rng_free(rng);
  }

  std::cout << "100% complete, time elapsed: " << time.elapsed() / 1000 << "s\n";
  std::cout.flush();
}

options_description PerPixelRenderer::options() const
{
  options_description opts("Per-pixel renderer");
  opts.add_options()
      ("ppr-x-samples", value<int>()->default_value(4), "Number of samples per pixel in x direction")
      ("ppr-y-samples", value<int>()->default_value(4), "Number of samples per pixel in y direction");
  return opts;
}
