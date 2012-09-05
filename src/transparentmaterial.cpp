#include "transparentmaterial.h"

#include "mathhelper.h"

TransparentMaterial::TransparentMaterial(float refractionCoeff) : refractionCoeff(refractionCoeff)
{
}

QVector3D TransparentMaterial::outDirection(QVector3D inDirection, QVector3D normal) const
{
    inDirection.normalize();
    normal.normalize();
    float cosIncomingAngle = QVector3D::dotProduct(normal, -inDirection);
    float coefficientRatio = 1 / refractionCoeff;
    float cosOutgoingAngle = sqrt(1 - coefficientRatio * coefficientRatio * (1 - cosIncomingAngle * cosIncomingAngle));
    if(abs(cosOutgoingAngle) >= 1)
    {
        return inDirection + 2 * cosIncomingAngle * normal;
    }
    else
    {
        return coefficientRatio * inDirection + signum(cosOutgoingAngle) * (coefficientRatio * cosIncomingAngle - cosOutgoingAngle) * normal;
    }
}
