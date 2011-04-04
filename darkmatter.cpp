#include "darkmatter.h"

Spectrum DarkMatter::shade(HitRecord&, const Light&) const
{
  return Spectrum(0, 0, 0);
}
