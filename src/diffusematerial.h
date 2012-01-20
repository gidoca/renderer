#ifndef DIFFUSEMATERIAL_H
#define DIFFUSEMATERIAL_H

#include "material.h"

class DiffuseMaterial : public Material
{
public:
    bool isMirror() const;
    bool isParticipating() const;
};

#endif // DIFFUSEMATERIAL_H
