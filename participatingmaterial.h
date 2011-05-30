#ifndef PARTICIPATINGMATERIAL_H
#define PARTICIPATINGMATERIAL_H

#include "material.h"
#include "spectrum.h"

#include <QVector3D>

class HitRecord;

class ParticipatingMaterial : public Material
{
public:
    bool isMirror() const;
    bool isParticipating() const;

    Spectrum shade(HitRecord&, QVector3D) const;
};

#endif // PARTICIPATINGMATERIAL_H
