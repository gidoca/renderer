#include "energyredistributionrenderer.h"

#include "unidipathtracingintegrator.h"
#include "path.h"
#include "camera.h"
#include "jitteredsampler.h"

#include <QSize>
#include <QPointF>
#include <QTime>

#include <algorithm>
#include <iostream>

using namespace boost::program_options;
using namespace cv;

void EnergyRedistributionRenderer::render(const Scene &scene, Mat &film, boost::program_options::variables_map vm)
{
  const int seed = getSeed(vm);

  gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set(rng, seed);
  const float ed = computeEd(scene, rng, vm["erpt-mutations"].as<int>());
  gsl_rng_free(rng);

  Size size = film.size();

  QTime time;
  time.start();

  #pragma omp parallel for schedule(dynamic)
  for(int i = 0; i < size.height; i++)
  {
    if(vm.count("verbose"))
    {
        std::cout << i * 100 / size.height << "% complete, ETA: " << time.elapsed() * (size.height - i) / ((i + 1) * 1000) << "s" << std::endl;
    }
    gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(rng, size.height * seed + i);
    for(int j = 0; j < size.width; j++)
    {
      QPointF pixelCoord(j, i);
      JitteredSampler multiSampler(vm["erpt-x-samples"].as<int>(), vm["erpt-y-samples"].as<int>(), rng);
      std::list<Sample> samples = multiSampler.getSamples();
      for(std::list<Sample>::iterator it = samples.begin(); it != samples.end(); it++)
      {
        QPointF point = pixelCoord + it->getSample();
        point.rx() /= size.width;
        point.ry() /= size.height;
        MetropolisSample initialSample(scene.light.size());
        initialSample.initAtPixel(point, rng);
        equalDispositionFlow(film, initialSample, *scene.object, scene.light, scene.camera, rng, ed, vm);
      }
    }
    gsl_rng_free(rng);
  }

}

void EnergyRedistributionRenderer::equalDispositionFlow(Mat &film, MetropolisSample initialSample, const Intersectable &scene, std::vector<const Light *>light, const Camera & camera, gsl_rng *rng, float ed, variables_map vm)
{
  UniDiPathTracingIntegrator integrator(0.1);
  Path initialPath = initialSample.cameraPathFromSample(scene, camera);
  Vec3f initialContrib = integrator.integrate(initialPath, scene, light, initialSample.lightSample1, initialSample.lightIndex);
  const int numMutations = vm["erpt-mutations"].as<int>();
  //TODO check (seems to always be 0 or 1)
  int numChains = (int)(gsl_rng_uniform(rng) + norm(initialContrib) / (numMutations * ed));
  Vec3f depVal = initialContrib * (ed / norm(initialContrib) / numChains);
  assert(norm(depVal) > 0);

  for(int i = 0; i < numChains; i++)
  {
    MetropolisSample y = initialSample;
    for(int j = 0; j < numMutations; j++)
    {
      MetropolisSample z = y;
      z.smallStep(rng);
      Path yPath = y.cameraPathFromSample(scene, camera);
      float yLum = norm(integrator.integrate(yPath, scene, light, y.lightSample1, y.lightIndex));
      Path zPath = z.cameraPathFromSample(scene, camera);
      float zLum = norm(integrator.integrate(zPath, scene, light, z.lightSample1, z.lightIndex));
      float q = std::min<float>(1, zLum / yLum);
      assert(q >= 0);
      if(q > gsl_rng_uniform(rng))
      {
        y = z;
      }
      int pixelX = (int)(y.cameraSample.getSample().x() * film.size().width);
      int pixelY = (int)(y.cameraSample.getSample().y() * film.size().height);
      #pragma omp critical
      film.at<Vec3f>(pixelY, pixelX) += depVal * (1.f / (vm["erpt-x-samples"].as<int>() * vm["erpt-y-samples"].as<int>()));
    }
  }
}

float EnergyRedistributionRenderer::computeEd(const Scene &scene, gsl_rng *rng, int pathsPerPixel)
{
  Vec3f ed;
  QSize size = scene.camera.getResolution();
  const UniDiPathTracingIntegrator integrator(0.1f);
  for(int i = 0; i < size.height(); i++)
  {
    for(int j = 0; j < size.width(); j++)
    {
      QPointF pixelCoord(j, i);
      const Ray ray = scene.camera.getRay(pixelCoord);
      ed += integrator.integrate(ray, *scene.object, scene.light, 4, rng);
    }
  }
  return norm(ed) / (size.height() * size.width() * pathsPerPixel);
}

boost::program_options::options_description EnergyRedistributionRenderer::options()
{
  options_description options("Energy Redistribution Renderer options");
  options.add_options()
    ("erpt-mutations", value<int>()->default_value(100), "number of mutations")
    ("erpt-x-samples", value<int>()->default_value(2), "number of samples per pixel in x direction")
    ("erpt-y-samples", value<int>()->default_value(2), "number of samples per pixel in y direction");
  return options;
}

const std::string EnergyRedistributionRenderer::name = "energyredist";
