#ifndef DARKMATTER_H
#define DARKMATTER_H

#include "material.h"

class DarkMatter : public Material
{
  public:
    Spectrum shade(HitRecord&, Light&, const Intersectable&) const;
};

#endif
