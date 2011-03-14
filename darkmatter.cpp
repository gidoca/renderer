#include "darkmatter.h"

Spectrum DarkMatter::shade(HitRecord&, Light&, const Intersectable&, int) const
{
  return Spectrum(0, 0, 0);
}
