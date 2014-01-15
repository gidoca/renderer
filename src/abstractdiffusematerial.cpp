#include "abstractdiffusematerial.h"

#include "sampler.h"

#include <QVector3D>

QVector3D AbstractDiffuseMaterial::outDirection(QVector3D, QVector3D surfaceNormal, Sample s, float &pdf) const
{
    return s.getCosineWeightedDirection(surfaceNormal, pdf);
}
