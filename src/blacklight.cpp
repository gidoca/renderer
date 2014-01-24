#include "blacklight.h"

#include "ray.h"

cv::Vec3f BlackLight::getIntensity(const HitRecord &, QVector3D &, const Intersectable &, const Sample &) const
{
    return cv::Vec3f();
}

// Always black, so it doesn't matter what ray we return
Ray BlackLight::getRandomRay(const Sample &, const Sample &, float &) const
{
    return Ray(QVector3D(0, 0, 0), QVector3D(1, 1, 1));
}

HitRecord BlackLight::intersect(Ray) const
{
    return HitRecord();
}

AxisAlignedBox* BlackLight::createBoundingBox()
{
    return new AxisAlignedBox(QVector3D(), QVector3D());
}
