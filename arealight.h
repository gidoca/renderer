#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "light.h"
#include "spectrum.h"
#include "jitteredsampler.h"

#include <QtGui/QVector3D>
#include <QtCore/QPointF>

class AreaLight : public Light
{
public:
    AreaLight(QVector3D origin, QVector3D uDirection, QVector3D vDirection, Spectrum intensity);

    Spectrum getIntensity(HitRecord & hit, QVector3D &direction, const Intersectable & scene, Sample sample) const;
    Ray getRandomRay(const Sample &sample, double &pdf) const;

private:
    QVector3D getLocation(QPointF p) const;

    QVector3D origin;
    QVector3D uDirection;
    QVector3D vDirection;
    Spectrum intensity;
    QVector3D normal;
};

#endif // AREALIGHT_H
