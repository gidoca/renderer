#include "abstractdiffusematerial.h"

#include "sampler.h"
#include "hitrecord.h"

#include <QVector3D>

QVector3D AbstractDiffuseMaterial::outDirection(const HitRecord &hit, Sample s, float &pdf, cv::Vec3f &brdf) const
{
    QVector3D normal = hit.getSurfaceNormal();
    if(QVector3D::dotProduct(normal.normalized(), hit.getRay().getDirection().normalized()) > 0) normal *= -1;
    QVector3D out = s.getCosineWeightedDirection(normal, pdf);
    brdf = this->brdf(hit, out);
    return out;
}
