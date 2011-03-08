#include "darkmatter.h"

Spectrum DarkMatter::shade(HitRecord &, Light &) const
{
  return Spectrum(0, 0, 0);
}
