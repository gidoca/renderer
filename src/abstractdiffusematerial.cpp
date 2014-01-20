#include "abstractdiffusematerial.h"

#include "sampler.h"
#include "hitrecord.h"

#include <QVector3D>

QVector3D AbstractDiffuseMaterial::outDirection(const HitRecord &hit, Sample s, float &pdf, cv::Vec3f &brdf) const
{
    QVector3D out = s.getCosineWeightedDirection(hit.getSurfaceNormal(), pdf);
    brdf = this->brdf(hit, out);
    return out;
}
