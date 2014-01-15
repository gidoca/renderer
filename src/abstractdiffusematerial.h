#ifndef ABSTRACTDIFFUSEMATERIAL_H
#define ABSTRACTDIFFUSEMATERIAL_H

#include "material.h"

class AbstractDiffuseMaterial : public Material
{
public:
    QVector3D outDirection(QVector3D, QVector3D surfaceNormal, Sample s, float &pdf) const;


};

#endif // ABSTRACTDIFFUSEMATERIAL_H
