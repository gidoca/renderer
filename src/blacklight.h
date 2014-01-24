#ifndef BLACKLIGHT_H
#define BLACKLIGHT_H

#include "global.h"

#include "light.h"
#include "axisalignedbox.h"

class BlackLight : public Light
{
public:
    cv::Vec3f getIntensity(const HitRecord &, QVector3D &, const Intersectable &, const Sample &) const;

    Ray getRandomRay(const Sample &, const Sample &, float &) const;

    HitRecord intersect(Ray) const;
    AxisAlignedBox* createBoundingBox();
};

#endif // BLACKLIGHT_H
