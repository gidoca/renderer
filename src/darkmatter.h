#ifndef DARKMATTER_H
#define DARKMATTER_H

#include "global.h"
#include "diffusematerial.h"

class DarkMatter : public DiffuseMaterial
{
public:
    Spectrum shade(HitRecord&, QVector3D direction) const;

    static DarkMatter* getInstance();

private:
    DarkMatter();
    static DarkMatter* instance;
};

inline DarkMatter* DarkMatter::getInstance()
{
  return instance;
}

#endif
