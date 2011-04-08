#include "mirrormaterial.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "light.h"

Spectrum MirrorMaterial::shade(HitRecord& , QVector3D direction) const
{
  return Spectrum();
}

bool MirrorMaterial::isMirror() const
{
  return true;
}
