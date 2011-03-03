#ifndef LIGHT_H
#define LIGHT_H

#include "spectrum.h"
#include "ray.h"

class Light
{

  public:
    virtual Spectrum getIntensity(Ray ray) const = 0;

};

#endif
