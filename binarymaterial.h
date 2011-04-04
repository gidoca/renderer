#ifndef BINARYMATERIAL_H
#define BINARYMATERIAL_H

#include "diffusematerial.h"


class BinaryMaterial:public DiffuseMaterial
{

  public:
    Spectrum shade (HitRecord&, const Light&) const;
    BinaryMaterial (Spectrum color);
    
  private:
    Spectrum color;
};

#endif // BINARYMATERIAL_H
