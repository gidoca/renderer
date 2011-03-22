#include "transparentmaterial.h"

#include "hitrecord.h"
#include "material.h"
#include "intersectable.h"

#include <iostream>
#include <cmath>
#include <limits>

TransparentMaterial::TransparentMaterial(double refractionCoefficient): MirrorMaterial(1), refractionCoefficient(refractionCoefficient)
{

}


Spectrum TransparentMaterial::shade(HitRecord& hit, Light& light, const Intersectable& scene, int depth) const
{
  if(depth > MAX_NUM_BOUNCES)
  {
    return Spectrum();
  }
  Spectrum mirrorComponent = MirrorMaterial::shade(hit, light, scene, depth + 1);
  QVector3D rayDirection = hit.getRay().getDirection().toVector3D().normalized();
  QVector3D surfaceNormal = hit.getSurfaceNormal().normalized();
  double cosAngle = QVector3D::dotProduct(-surfaceNormal, rayDirection);
  double angle = acos(cosAngle);
  if(sin(angle) * refractionCoefficient > 1) return mirrorComponent;
  double outAngle = asin(refractionCoefficient * sin(angle));
  QVector3D refractedDirectionFront = refractionCoefficient * rayDirection + (refractionCoefficient * cosAngle - cos(outAngle)) * hit.getSurfaceNormal();
  QVector3D refractedDirectionBack = 1 / refractionCoefficient * rayDirection + (1 / refractionCoefficient * cosAngle - cos(outAngle)) * hit.getSurfaceNormal();
  refractedDirectionFront.normalize();
  refractedDirectionBack.normalize();
  QVector3D newOrigin = hit.getIntersectingPoint();
  Ray newFrontRay = Ray(newOrigin, refractedDirectionFront);
  Ray newBackRay = Ray(newOrigin, -refractedDirectionBack);
  HitRecord newHitFront = scene.intersect(newFrontRay);
  HitRecord newHitBack = scene.intersect(newBackRay, -EPSILON, -std::numeric_limits< double >::infinity());
  HitRecord newHit;
  if(newHitFront.getRayParameter() < newHitBack.getRayParameter())
  {
    newHit = newHitFront;
  }
  else
  {
    newHit = newHitBack;
  }
  double f = pow(1 - refractionCoefficient, 2) / pow(1 + refractionCoefficient, 2);
  double F = f + (1 - f) * pow(1 - QVector3D::dotProduct(rayDirection, -surfaceNormal), 5);
  return (1 - F) * newHit.getMaterial().shade(newHit, light, scene, depth + 1) + F * mirrorComponent;
}

