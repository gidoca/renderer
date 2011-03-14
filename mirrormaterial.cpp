#include "mirrormaterial.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "light.h"

Spectrum MirrorMaterial::shade(HitRecord& hit, Light& light, const Intersectable& scene, int depth) const
{
  if(depth > MAX_NUM_BOUNCES)
  {
    return Spectrum();
  }
  
  QVector3D oldRayDirection = hit.getRay().getDirection().toVector3D();
  QVector3D surfaceNormal = hit.getSurfaceNormal();
  surfaceNormal.normalize();
  QVector3D newDirection = oldRayDirection - 2 * QVector3D::dotProduct(oldRayDirection, surfaceNormal) * surfaceNormal;
  Ray mirrorRay(hit.getIntersectingPoint(), newDirection);
  HitRecord newHit = scene.intersect(mirrorRay);
  return newHit.getMaterial().shade(newHit, light, scene, depth + 1);
}

