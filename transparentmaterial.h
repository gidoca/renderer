#ifndef TRANSPARENTMATERIAL_H
#define TRANSPARENTMATERIAL_H

#include "mirrormaterial.h"

class TransparentMaterial : public MirrorMaterial
{

  public:
    TransparentMaterial(float refractionCoefficient);
    virtual Spectrum shade(HitRecord& hit, Light& light, const Intersectable& scene, int depth) const;
    
  private:
    float refractionCoefficient;
};

#endif // TRANSPARENTMATERIAL_H
