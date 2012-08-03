#include "transparentmaterial.h"

TransparentMaterial::TransparentMaterial(float refractionCoeff) : refractionCoeff(refractionCoeff)
{
}

QVector3D TransparentMaterial::outDirection(QVector3D inDirection, QVector3D normal) const
{
//    bool incomingAngle = QVector3D::dotProduct(inDirection, normal);
//    float coefficientRatio = incomingAngle < 0 ? 1 / refractionCoeff : refractionCoeff;
    return QVector3D();
}
