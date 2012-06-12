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
using namespace cv;

void PerPixelRenderer::render(const Scene & scene, cv::Mat & film, const boost::program_options::variables_map vm)
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
  for(int i = 0; i < film.size().height; i++)
  {
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(rng, film.size().height * seed + i);
    if(vm.count("verbose"))
    {
      std::cout << i * 100 / film.size().height << "% complete, ETA: " << time.elapsed() * (film.size().height - i) / ((i + 1) * 1000) << "s" << std::endl;
    }
    Vec3f * scanline = film.ptr<Vec3f>(i);
    for(int j = 0; j < film.size().width; j++)
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
        Vec3f s = integrator->integrate(ray, *scene.object, scene.light, rng);
        assert(!isnan(s[0]) && !isnan(s[1]) && !isnan(s[2]));
        assert(s[0] >= 0 && s[1] >= 0 && s[2] >= 0);
        scanline[j] += s * (1.f / samples.size());
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
