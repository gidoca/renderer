#include "participatingmaterial.h"

bool ParticipatingMaterial::isMirror() const
{
  return false;
}

bool ParticipatingMaterial::isParticipating() const
{
  return true;
}

Spectrum ParticipatingMaterial::shade(HitRecord &, QVector3D) const
{
  return .004 * Spectrum(1, 1, 1);
}
