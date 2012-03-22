#ifndef DARKMATTER_H
#define DARKMATTER_H

#include "global.h"
#include "diffusematerial.h"

class DarkMatter : public Material
{
public:
    Spectrum shade(const HitRecord&, QVector3D direction) const;
    virtual bool isMirror() const;

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
