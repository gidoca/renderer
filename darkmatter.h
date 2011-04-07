#ifndef DARKMATTER_H
#define DARKMATTER_H

#include "diffusematerial.h"

#include <QSharedPointer>

class DarkMatter : public DiffuseMaterial
{
public:
    Spectrum shade(HitRecord&, QVector3D direction) const;

    static QSharedPointer<DarkMatter> getInstance();

private:
    DarkMatter();
    static QSharedPointer<DarkMatter> instance;
};

inline QSharedPointer<DarkMatter> DarkMatter::getInstance()
{
  return instance;
}

#endif
