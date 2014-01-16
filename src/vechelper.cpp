#include "vechelper.h"

QVector3D reflect(const QVector3D &v, const QVector3D &normal)
{
    return v - 2 * QVector3D::dotProduct(v, normal) * normal;
}
