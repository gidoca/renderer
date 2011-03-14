#include "darkmatter.h"

Spectrum DarkMatter::shade(HitRecord&, Light&, const Intersectable&) const
{
  return Spectrum(0, 0, 0);
}
