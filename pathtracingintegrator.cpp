#include "pathtracingintegrator.h"

#include "hitrecord.h"
#include "jitteredsampler.h"
#include "intersectable.h"
#include "light.h"
#include "path.h"
#include "participatingmaterial.h"

#include <list>
#include <iostream>
#include <cmath>
#include <cassert>

Path PathTracingIntegrator::createPath(const Ray& primaryRay, const Intersectable &scene, Spectrum alpha) const
{
  Path result;
  HitRecord hit = scene.intersect(primaryRay);
  JitteredSampler sampler(1, 1);
  for(int i = 0; i < MAX_DEPTH; i++)
  {
    if(!hit.intersects()) return result;

#ifdef ROUSSIAN_ROULETTE
    const float terminationProb = 0.5;
    if(i >= 2 && qrand() < RAND_MAX * terminationProb) return result;
    float russianRoulettePdf = (i < 2 ? 1 : 1 - terminationProb);
#else
    const float russianRoulettePdf = 1;
#endif

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
    else if(hit.getMaterial().isParticipating())
    {
      outDirection = hit.getRay().getDirection();
      QVector3D initialLocation = hit.getIntersectingPoint();
      QVector3D currentLocation = initialLocation;
      Ray outRay(currentLocation, outDirection);
      hit = scene.intersect(outRay);
//      QVector3D dt = (hit.getIntersectingPoint() - currentLocation) / PARTICIPATING_SAMPLES;
      QVector3D dist = hit.getIntersectingPoint() - currentLocation;
      for(float t = 0; t < dist.length(); t += dt)
      {
        currentLocation = initialLocation + t * dist.normalized();
        Ray marchingRay(currentLocation, outDirection.normalized());
        HitRecord currentHit((currentLocation - initialLocation).length(), marchingRay, QSharedPointer<Material>(new ParticipatingMaterial()), QVector3D());
        result.alphaValues.push_back(alpha);
        result.hitRecords.push_back(currentHit);
      }
      alpha *= Spectrum(1, 1, 1) - .0005 * Spectrum(1, 1, 1) * dt;
    }
    else
    {
      outDirection = sampler.getSamples().front().getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
      Spectrum brdf = hit.getMaterial().shade(hit, -outDirection);
      float cos = QVector3D::dotProduct(outDirection.normalized(), hit.getSurfaceNormal().normalized());
      assert(cos >= 0);
      assert(pdf >= 0);
      assert(brdf.x() >= 0 && brdf.y() >= 0 && brdf.z() >= 0);
      alpha *= brdf * cos / pdf / russianRoulettePdf;
    }
    hit = scene.intersect(Ray(hit.getIntersectingPoint(), outDirection));
  }
  return result;
}
