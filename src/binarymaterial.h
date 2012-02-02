#ifndef BINARYMATERIAL_H
#define BINARYMATERIAL_H

#include "global.h"
#include "diffusematerial.h"

class BinaryMaterial:public DiffuseMaterial
{

  public:
    BinaryMaterial(Spectrum color) : color(color)
    {

    }

    Spectrum shade (const HitRecord&, QVector3D) const;
    
  private:
    Spectrum color;
};

#endif // BINARYMATERIAL_H
