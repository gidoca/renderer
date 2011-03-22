#include "transparentmaterial.h"

#include "hitrecord.h"
#include "material.h"
#include "intersectable.h"

#include <iostream>
#include <cmath>

TransparentMaterial::TransparentMaterial(double refractionCoefficient): MirrorMaterial(1), refractionCoefficient(1 / refractionCoefficient)
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
  QVector3D refractedDirection = refractionCoefficient * rayDirection + (refractionCoefficient * cosAngle - cos(outAngle)) * hit.getSurfaceNormal();
  refractedDirection.normalize();
  QVector3D newOrigin = hit.getIntersectingPoint();
  HitRecord newHit = scene.intersect(Ray(newOrigin, refractedDirection));
  double f = pow(1 - refractionCoefficient, 2) / pow(1 + refractionCoefficient, 2);
  double F = f + (1 - f) * pow(1 - QVector3D::dotProduct(rayDirection, -surfaceNormal), 5);
  return (1 - F) * newHit.getMaterial().shade(newHit, light, scene, depth + 1) + F * mirrorComponent;
}

