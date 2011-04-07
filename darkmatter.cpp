#include "darkmatter.h"

DarkMatter::DarkMatter()
{

}

Spectrum DarkMatter::shade(HitRecord&, QVector3D) const
{
  return Spectrum(0, 0, 0);
}

QSharedPointer<DarkMatter> DarkMatter::instance(new DarkMatter());
