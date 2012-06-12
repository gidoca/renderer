#ifndef CONELIGHT_H
#define CONELIGHT_H

#include "global.h"
#include "light.h"
#include "spectrum.h"

#include <QVector3D>

class ConeLight : public Light
{
public:
    ConeLight(QVector3D location, QVector3D direction, float openingAngle, cv::Vec3f intensity) : location(location), direction(direction), openingAngle(openingAngle), intensity(intensity)
    {
    }

    cv::Vec3f getIntensity(const QVector3D &at, QVector3D &direction, const Intersectable &scene, const Sample &sample) const;
    Ray getRandomRay(const Sample &sample, const Sample&, float &pdf) const;

private:
    QVector3D location, direction;
    float openingAngle;
    cv::Vec3f intensity;
};

#endif // CONELIGHT_H
