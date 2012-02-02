#include "mirrormaterial.h"

#include "hitrecord.h"
#include "intersectable.h"
#include "light.h"

Spectrum MirrorMaterial::shade(const HitRecord& , QVector3D) const
{
  return Spectrum();
}

bool MirrorMaterial::isMirror() const
{
  return true;
}

bool MirrorMaterial::isParticipating() const
{
  return false;
}
