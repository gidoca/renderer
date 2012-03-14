#include "perpixelrenderer.h"

#include "intersectable.h"
#include "camera.h"
#include "light.h"
#include "unidipathtracingintegrator.h"
#include "bidipathtracingintegrator.h"
#include "jitteredsampler.h"
#include "integrator.h"

#include <iostream>
#include <cassert>
#include <cmath>

#include <QTime>

#include <gsl/gsl_rng.h>

using namespace boost::program_options;
using namespace std;

void PerPixelRenderer::render(const Scene & scene, Film & film, const boost::program_options::variables_map vm)
{
  Integrator * integrator;
  if(vm["pt-integrator"].as<string>() == "unidi")
  {
    integrator = new UniDiPathTracingIntegrator(vm["pt-termination-prob"].as<float>());
  }
  else
  {
    integrator = new BiDiPathTracingIntegrator();
  }

  QTime time;
  time.start();

  int seed = getSeed(vm);
  
  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < film.height(); i++)
  {
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(rng, film.height() * seed + i);
    if(vm.count("verbose"))
    {
      std::cout << i * 100 / film.height() << "% complete, ETA: " << time.elapsed() * (film.height() - i) / ((i + 1) * 1000) << "s" << std::endl;
    }
    Spectrum * scanline = film[i];
    for(int j = 0; j < film.width(); j++)
    {
#ifndef NDEBUG
      //Black pixel
      if(j == 84 && i == 13)
        std::cout << "";
#endif
      JitteredSampler multiSampler(vm["pt-x-samples"].as<int>(), vm["pt-y-samples"].as<int>(), rng);
      std::list<Sample> samples = multiSampler.getSamples();
      QPointF point = QPoint(j, i);
      for(std::list<Sample>::iterator it = samples.begin(); it != samples.end(); it++)
      {
        QPointF samplePoint = point + it->getSample();
        Ray ray = scene.camera.getRay(samplePoint);
        Spectrum s = integrator->integrate(ray, *scene.object, scene.light, rng);
        assert(!isnan(s.x()) && !isnan(s.y()) && !isnan(s.z()));
        assert(s.x() >= 0 && s.y() >= 0 && s.z() >= 0);
        scanline[j] += s / samples.size();
      }
    }
    gsl_rng_free(rng);
  }
}

options_description PerPixelRenderer::options()
{
  options_description opts("Path tracer options");
  opts.add_options()
      ("pt-integrator", value<string>()->default_value("unidi"), "the path tracing method (unidi or bidi)")
      ("pt-termination-prob", value<float>()->default_value(0.5f, "0.5"), "the roussian roulette path termination probability (use 0 to disable roussian roulette path termination)")
      ("pt-x-samples", value<int>()->default_value(4), "number of samples per pixel in x direction")
      ("pt-y-samples", value<int>()->default_value(4), "number of samples per pixel in y direction")
      ("pt-fixed-seed", "use a fixed seed for the RNG to make the resulting image deterministic");
  return opts;
}

const std::string PerPixelRenderer::name = "pathtracing";
