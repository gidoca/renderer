#ifndef BINARYMATERIAL_H
#define BINARYMATERIAL_H

#include <../../mnt/files/uni/info/rendering/renderingproject/material.h>


class BinaryMaterial:public Material
{

  public:
    virtual Spectrum shade (HitRecord & hit, Light & light) const;
    BinaryMaterial (Spectrum color);
    
  private:
    Spectrum color;
};

#endif // BINARYMATERIAL_H
