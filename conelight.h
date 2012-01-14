#ifndef CONELIGHT_H
#define CONELIGHT_H

#include "light.h"
#include "spectrum.h"

#include <QVector3D>

class ConeLight : public Light
{
public:
    ConeLight(QVector3D location, QVector3D direction, float openingAngle, Spectrum intensity) : location(location), direction(direction), openingAngle(openingAngle), intensity(intensity)
    {
    }

    Spectrum getIntensity(HitRecord &hit, QVector3D &direction, const Intersectable &scene, const Sample &sample) const;
    Ray getRandomRay(const Sample &sample, float &pdf) const;

private:
    QVector3D location, direction;
    float openingAngle;
    Spectrum intensity;
};

#endif // CONELIGHT_H
