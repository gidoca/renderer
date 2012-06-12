#include "mirrormaterial.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "light.h"

cv::Vec3f MirrorMaterial::shade(const HitRecord& , QVector3D) const
{
  return cv::Vec3f();
}

bool MirrorMaterial::isMirror() const
{
  return true;
}
