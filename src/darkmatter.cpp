#include "darkmatter.h"

DarkMatter::DarkMatter()
{

}

Spectrum DarkMatter::shade(const HitRecord&, QVector3D) const
{
  return Spectrum(0, 0, 0);
}

bool DarkMatter::isMirror() const
{
  return false;
}

DarkMatter* DarkMatter::instance(new DarkMatter());
