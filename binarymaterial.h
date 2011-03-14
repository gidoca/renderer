#ifndef BINARYMATERIAL_H
#define BINARYMATERIAL_H

#include "material.h"


class BinaryMaterial:public Material
{

  public:
    virtual Spectrum shade (HitRecord&, Light&, const Intersectable&, int) const;
    BinaryMaterial (Spectrum color);
    
  private:
    Spectrum color;
};

#endif // BINARYMATERIAL_H
