#include "renderer.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "jitteredsampler.h"

#include "energyredistributionrenderer.h"
#include "perpixelrenderer.h"
#include "metropolisrenderer.h"
#include "metropolisfltrenderer.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include <cmath>
#include <cassert>
#include <algorithm>

#include <QTime>

#include <opencv2/core/core.hpp>

using namespace std;

struct initializer
{
  Renderer **renderer;
  std::string name;
  initializer(Renderer **renderer, std::string name) : renderer(renderer), name(name) {}

  template< typename R > void operator()(R)
  {
    if(name == R::name)
    {
      *renderer = new R();
    }
  }
};

Renderer *getRendererByName(string name)
{
  Renderer * renderer = 0;
  boost::mpl::for_each<renderers>(initializer(&renderer, name));
  return renderer;
}

Path Renderer::createPath(const Ray &primaryRay, const Intersectable &scene, gsl_rng *rng, cv::Vec3f initialAlpha, float terminationProb)
{
    JitteredSampler sampler(1, 1, rng);
    Sample pathSamples[MAX_DEPTH];
    int pathLength;
    if(terminationProb > 0)
    {
      pathLength = max(1, min<int>(gsl_ran_negative_binomial(rng, terminationProb, 1), MAX_DEPTH));
    }
    else
    {
      terminationProb = 1;
      pathLength = MAX_DEPTH;
    }
    for(int i = 0; i < pathLength; i++)
    {
      pathSamples[i] = sampler.getSamples().front();
    }
    return createPath(primaryRay, scene, pathSamples, initialAlpha, pathLength, terminationProb);
}

Path Renderer::createPath(const Ray& primaryRay, const Intersectable &scene, Sample pathSamples[], cv::Vec3f alpha, int pathLength, float russianRoulettePdf)
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
  for(int i = 0; i < pathLength; i++)
  {
    if(!hit.intersects()) return result;

//    assert(!isnan(alpha.x()) && !isnan(alpha.y()) && !isnan(alpha.z()));
    result.alphaValues.push_back(alpha);
    result.hitRecords.push_back(hit);

    float pdf;
    QVector3D outDirection;
    if(hit.getMaterial().isMirror())
    {
      QVector3D oldRayDirection = hit.getRay().getDirection();
      QVector3D surfaceNormal = hit.getSurfaceNormal();
      surfaceNormal.normalize();
      outDirection = oldRayDirection - 2 * QVector3D::dotProduct(oldRayDirection, surfaceNormal) * surfaceNormal;
    }
    else
    {
      outDirection = pathSamples[i].getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
      if(pdf == 0) return result;
      cv::Vec3f brdf = hit.getMaterial().shade(hit, -outDirection);
      float cos = QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized());
      assert(cos >= 0 && !isnan(pdf));
      assert(pdf >= 0 && !isnan(pdf));
//      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
//      assert(!isnan(brdf.x()) && !isnan(brdf.y()) && !isnan(brdf.z()));
      alpha = alpha.mul(brdf) * (cos / pdf / russianRoulettePdf);
    }
    hit = scene.intersect(Ray(hit.getIntersectingPoint(), outDirection));
  }
  return result;
}

boost::program_options::options_description Renderer::options()
{
  return boost::program_options::options_description("Renderer");
}

int Renderer::getSeed(boost::program_options::variables_map vm)
{
  if(vm.count("pt-fixed-seed"))
  {
    return 0;
  }
  else
  {
    return QTime::currentTime().msec() + 1000 * QTime::currentTime().second();
  }
}
