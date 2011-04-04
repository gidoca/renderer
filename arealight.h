#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "light.h"
#include "spectrum.h"
#include "jitteredsampler.h"

#include <QVector3D>

class AreaLight : public Light
{
public:
    AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, Spectrum intensity);

    Spectrum getIntensity(HitRecord & hit, QVector3D &direction, const Intersectable & scene, QPointF sample) const;

private:
    QVector3D origin;
    QVector3D uDirection;
    QVector3D vDirection;
    Spectrum intensity;
    QVector3D normal;
};

#endif // AREALIGHT_H
