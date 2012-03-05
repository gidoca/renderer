#include "energyredistributionrenderer.h"

#include "unidipathtracingintegrator.h"
#include "path.h"
#include "camera.h"
#include "jitteredsampler.h"

#include <QSize>
#include <QPointF>

#include <algorithm>

void EnergyRedistributionRenderer::render(const Scene &scene, Film &film, boost::program_options::variables_map vm)
{
  gsl_rng *rng = gsl_rng_alloc(gsl_rng_taus);
  int seed = getSeed(vm);
  gsl_rng_set(rng, seed);

  float ed = computeEd(scene, rng, 5);

  QSize size = film.getSize();

  for(int i = 0; i < size.height(); i++)
  {
    for(int j = 0; j < size.width(); j++)
    {
      QPointF pixelCoord(j, i);
      JitteredSampler multiSampler(4, 4, rng);
      std::list<Sample> samples = multiSampler.getSamples();
      for(std::list<Sample>::iterator it = samples.begin(); it != samples.end(); it++)
      {
        QPointF point = pixelCoord + it->getSample();
        point.rx() /= size.width();
        point.ry() /= size.height();
        MetropolisSample initialSample(scene.light.size());
        initialSample.initAtPixel(point, rng);
        equalDispositionFlow(film, initialSample, *scene.object, scene.light, scene.camera, rng, ed);
      }
    }
  }

  gsl_rng_free(rng);
}

void EnergyRedistributionRenderer::equalDispositionFlow(Film &film, MetropolisSample initialSample, const Intersectable &scene, std::vector<const Light *>light, const Camera & camera, gsl_rng *rng, float ed)
{
  UniDiPathTracingIntegrator integrator(0.1);
  Path initialPath = initialSample.cameraPathFromSample(scene, camera);
  Spectrum initialContrib = integrator.integrate(initialPath, scene, light, initialSample.lightSample1, initialSample.lightIndex);
  Spectrum depVal = initialContrib / initialContrib.length() * ed / 4;
  const int numMutations = 3;
  int numChains = (int)(gsl_rng_uniform(rng) + initialContrib.length() / (numMutations * ed));

  MetropolisSample y(light.size());

  for(int i = 0; i < numChains; i++)
  {
    y = initialSample;
    for(int j = 0; j < numMutations; j++)
    {
      MetropolisSample z = y;
      z.smallStep(rng);
      Path yPath = y.cameraPathFromSample(scene, camera);
      float yLum = integrator.integrate(yPath, scene, light, y.lightSample1, y.lightIndex).length();
      Path zPath = z.cameraPathFromSample(scene, camera);
      float zLum = integrator.integrate(zPath, scene, light, z.lightSample1, z.lightIndex).length();
      float q = std::min<float>(1, zLum / yLum);
      if(q > gsl_rng_uniform(rng))
      {
        y = z;
      }
      int pixelX = (int)(y.cameraSample.getSample().x() * film.width());
      int pixelY = (int)(y.cameraSample.getSample().y() * film.height());
      film[pixelY][pixelX] += depVal;
    }
  }
}

float EnergyRedistributionRenderer::computeEd(const Scene &scene, gsl_rng *rng, int pathsPerPixel)
{
  Spectrum ed;
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
  return ed.length() / (size.height() * size.width() * pathsPerPixel);
}
